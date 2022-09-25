/**********************************************************************************************
*
*   raylib-imagedraw - Extra image draw methods for raylib.
*
*   Copyright 2021 Rob Loach (@RobLoach)
*
*   DEPENDENCIES:
*       raylib https://www.raylib.com/
*
*   LICENSE: zlib/libpng
*
*   raylib-imagedraw is licensed under an unmodified zlib/libpng license, which is an OSI-certified,
*   BSD-like license that allows static linking with closed source software:
*
*   This software is provided "as-is", without any express or implied warranty. In no event
*   will the authors be held liable for any damages arising from the use of this software.
*
*   Permission is granted to anyone to use this software for any purpose, including commercial
*   applications, and to alter it and redistribute it freely, subject to the following restrictions:
*
*     1. The origin of this software must not be misrepresented; you must not claim that you
*     wrote the original software. If you use this software in a product, an acknowledgment
*     in the product documentation would be appreciated but is not required.
*
*     2. Altered source versions must be plainly marked as such, and must not be misrepresented
*     as being the original software.
*
*     3. This notice may not be removed or altered from any source distribution.
*
**********************************************************************************************/

#ifndef INCLUDE_RAYLIB_IMAGEDRAW_H_
#define INCLUDE_RAYLIB_IMAGEDRAW_H_

#ifdef __cplusplus
extern "C" {
#endif

RLAPI void ImageDrawLineHorizontal(Image* dst, int posX, int posY, int width, Color color);
RLAPI void ImageDrawLineVertical(Image* dst, int posX, int posY, int height, Color color);
RLAPI void ImageDrawPoly(Image* dst, Vector2* points, int pointCount, Color color);
RLAPI void ImageDrawPolyLines(Image* dst, Vector2* points, int pointCount, Color color);
RLAPI void ImageDrawEllipse(Image* dst, int centerX, int centerY, int radiusX, int radiusY, Color color);
RLAPI void ImageDrawEllipseLines(Image* dst, int centerX, int centerY, int radiusX, int radiusY, Color color);
RLAPI void ImageDrawImage(Image* dst, Image src, int posX, int posY, Color tint);
RLAPI Image ImageRotate(Image src, float angle);
RLAPI void DrawImage(Image image, int posX, int posY);

#ifdef __cplusplus
}
#endif

#endif  // INCLUDE_RAYLIB_IMAGEDRAW_H_

#ifdef RAYLIB_IMAGEDRAW_IMPLEMENTATION
#ifndef RAYLIB_IMAGEDRAW_IMPLEMENTATION_ONCE
#define RAYLIB_IMAGEDRAW_IMPLEMENTATION_ONCE

#ifdef __cplusplus
extern "C" {
#endif

#ifndef RAYLIB_IMAGEDRAW_MIN
#define RAYLIB_IMAGEDRAW_MIN(a, b) (((a) < (b)) ? (a) : (b))
#endif
#ifndef RAYLIB_IMAGEDRAW_MAX
#define RAYLIB_IMAGEDRAW_MAX(a, b) (((a) > (b)) ? (a) : (b))
#endif

#ifndef RAYLIB_IMAGEDRAW_COSF
#include <math.h>
#define RAYLIB_IMAGEDRAW_COSF cosf
#endif

#ifndef RAYLIB_IMAGEDRAW_SINF
#include <math.h>
#define RAYLIB_IMAGEDRAW_SINF sinf
#endif

#ifndef RAYLIB_IMAGEDRAW_PI
#define RAYLIB_IMAGEDRAW_PI 3.14159265358979323846f
#endif

#ifndef RAYLIB_IMAGEDRAW_SQRTF
#include <math.h>
#define RAYLIB_IMAGEDRAW_SQRTF sqrtf
#endif

#ifndef RAYLIB_IMAGEDRAW_ATAN2F
#include <math.h>
#define RAYLIB_IMAGEDRAW_ATAN2F atan2f
#endif

void ImageDrawLineHorizontal(Image* dst, int posX, int posY, int width, Color color) {
    Rectangle rect = {(float)posX, (float)posY, (float)width, 1};
    ImageDrawRectangleRec(dst, rect, color);
}

void ImageDrawLineVertical(Image* dst, int posX, int posY, int height, Color color) {
    for (int y = 0; y < height; y++) {
        ImageDrawPixel(dst, posX, posY + y, color);
    }
}

void ImageDrawPolyLines(Image* dst, Vector2* points, int pointCount, Color color) {
    for (int i = 0; i < pointCount - 1; i++) {
        ImageDrawLineV(dst, points[i], points[i + 1], color);
    }
    ImageDrawLineV(dst, points[0], points[pointCount - 1], color);
}

void ImageDrawPoly(Image* dst, Vector2* points, int pointCount, Color color) {
    int nodes[64];
    int y;
    float x0, y0, x1, y1;
    int miny = dst->height;
    int maxy = 0;

    for (int i = 0; i < pointCount; i++) {
        if (miny > points[i].y) {
            miny = (int)points[i].y;
        }
        if (maxy < points[i].y) {
            maxy = (int)points[i].y;
        }
    }

    // Loop through the rows of the image.
    for (y = miny; y < maxy; y++) {

        // Build a list of nodes.
        int count = 0;
        int j = pointCount - 1;

        for (int i = 0; i < pointCount; i++) {
            x0 = points[i].x;
            y0 = points[i].y;
            x1 = points[j].x;
            y1 = points[j].y;

            if (
                (y0 < (float)y && y1 >= (float)y) ||
                (y1 < (float)y && y0 >= (float)y)
            ) {
                nodes[count] = (int)(x0 + ((float)y - y0) / (y1 - y0) * (x1 - x0));
                count++;
            }
            j = i;
        }

        // Sort the nodes, via a simple “Bubble” sort.
        int i = 0;
        while (i < count - 1) {
            if (nodes[i] > nodes[i + 1]) {
                int swap = nodes[i];
                nodes[i] = nodes[i + 1];
                nodes[i + 1] = swap;
                if (i) {
                    i--;
                }
            } else {
                i++;
            }
        }

        // Draw lines between nodes.
        for (int i = 0; i < count; i += 2) {
            int width = nodes[i + 1] - nodes[i];
            ImageDrawLineHorizontal(dst, nodes[i], y, width, color);
        }
    }
}

void DrawImage(Image image, int posX, int posY) {
    for (int x = 0; x < image.width; x++) {
        for (int y = 0; y < image.height; y++) {
            DrawPixel(posX + x, posY + y, GetImageColor(image, x, y));
        }
    }
}

void ImageDrawEllipseLines(Image* dst, int centerX, int centerY, int radiusX, int radiusY, Color color) {
    int wx = 0, wy = radiusY;
    int asq = radiusX * radiusX;
    int bsq = radiusY * radiusY;
    int xa = 0, ya = asq * 2 * radiusY;
    int t = asq / 4 - asq * radiusY;

    ImageDrawPixel(dst, centerX, centerY + radiusY, color);
    ImageDrawPixel(dst, centerX, centerY - radiusY, color);

    while (1) {
        t += xa + bsq;
        if (t >= 0) {
            ya -= asq * 2;
            t -= ya;
            wy--;
        }
        xa += bsq * 2;
        wx++;
        if (xa >= ya) {
            break;
        }
        ImageDrawPixel(dst, centerX + wx, centerY - wy, color);
        ImageDrawPixel(dst, centerX - wx, centerY - wy, color);
        ImageDrawPixel(dst, centerX + wx, centerY + wy, color);
        ImageDrawPixel(dst, centerX - wx, centerY + wy, color);
    }

    ImageDrawPixel(dst, centerX + radiusX, centerY, color);
    ImageDrawPixel(dst, centerX - radiusX, centerY, color);

    wx = radiusX;
    wy = 0;
    xa = bsq * 2 * radiusX;
    ya = 0;
    t = bsq / 4 - bsq * radiusX;

    while (1) {
        t += ya + asq;
        if (t >= 0) {
            xa -= bsq * 2;
            t = t - xa;
            wx--;
        }
        ya += asq * 2;
        wy++;
        if (ya > xa) {
            break;
        }
        ImageDrawPixel(dst, centerX + wx, centerY - wy, color);
        ImageDrawPixel(dst, centerX - wx, centerY - wy, color);
        ImageDrawPixel(dst, centerX + wx, centerY + wy, color);
        ImageDrawPixel(dst, centerX - wx, centerY + wy, color);
    }
}

void ImageDrawEllipse(Image* dst, int centerX, int centerY, int radiusX, int radiusY, Color color) {
    int wx = 0, wy = radiusY;
    int asq = radiusX * radiusX;
    int xa = 0, ya = asq * 2 * radiusY;
    int t = asq / 4 - asq * radiusY;
    int bsq = radiusY * radiusY;

    ImageDrawPixel(dst, centerX, centerY + radiusY, color);
    ImageDrawPixel(dst, centerX, centerY - radiusY, color);

    while (1) {
        t += xa + bsq;
        if (t >= 0) {
            ya -= asq * 2;
            t -= ya;
            wy--;
        }
        xa += bsq * 2;
        wx++;
        if (xa >= ya) {
            break;
        }
        ImageDrawLineHorizontal(dst, centerX - wx, centerY - wy, wx * 2, color);
        ImageDrawLineHorizontal(dst, centerX - wx, centerY + wy, wx * 2, color);
    }

    ImageDrawLineHorizontal(dst, centerX - radiusX, centerY, radiusX * 2, color);

    wx = radiusX;
    wy = 0;
    xa = bsq * 2 * radiusX;
    ya = 0;
    t = bsq / 4 - bsq * radiusX;

    while (1) {
        t += ya + asq;
        if (t >= 0) {
            xa -= bsq * 2;
            t = t - xa;
            wx--;
        }
        ya += asq * 2;
        wy++;
        if (ya > xa) {
            break;
        }
        ImageDrawLineHorizontal(dst, centerX - wx, centerY - wy, wx * 2, color);
        ImageDrawLineHorizontal(dst, centerX - wx, centerY + wy, wx * 2, color);
    }
}

void ImageDrawTriangleLines(Image* dst, Vector2 point1, Vector2 point2, Vector2 point3, Color color) {
    Vector2 points[3] = {point1, point2, point3};
    ImageDrawPolyLines(dst, points, 3, color);
}

void ImageDrawTriangle(Image* dst, Vector2 point1, Vector2 point2, Vector2 point3, Color color) {
    Vector2 points[3] = {point1, point2, point3};
    ImageDrawPoly(dst, points, 3, color);
}

Image ImageRotate(Image src, float angle) {
    // TODO: Determine if we need to rotate the image by -angle or angle
    float radians = -angle * (RAYLIB_IMAGEDRAW_PI / 180.0f);
    int iDiagonal = (int)RAYLIB_IMAGEDRAW_SQRTF((float)(src.width * src.width + src.height * src.height));
    int iCentreX = src.width / 2;
    int iCentreY = src.height / 2;
    int iDestCentre = iDiagonal / 2;
    Image out = GenImageColor(iDiagonal, iDiagonal, BLANK);

    for (int i = 0; i < iDiagonal; ++i) {
        for (int j = 0; j < iDiagonal; ++j) {
            int x = j - iDestCentre;
            int y = iDestCentre - i;

            float fDistance = RAYLIB_IMAGEDRAW_SQRTF((float)(x * x + y * y));
            float fPolarAngle = 0.0;
            if (x == 0) {
                if (y == 0) {
                    ImageDrawPixel(&out, j, i, GetImageColor(src, iCentreX, iCentreY));
                    continue;
                }
                else if (y < 0) {
                    fPolarAngle = 1.5f * RAYLIB_IMAGEDRAW_PI;
                }
                else {
                    fPolarAngle = 0.5f * RAYLIB_IMAGEDRAW_PI;
                }
            }
            else {
                fPolarAngle = RAYLIB_IMAGEDRAW_ATAN2F((float)y, (float)x);
            }

            fPolarAngle -= radians;

            float fTrueX = fDistance * RAYLIB_IMAGEDRAW_COSF(fPolarAngle);
            float fTrueY = fDistance * RAYLIB_IMAGEDRAW_SINF(fPolarAngle);

            fTrueX = fTrueX + (float)iCentreX;
            fTrueY = (float)iCentreY - fTrueY;

            int iFloorX = (int)fTrueX;
            int iFloorY = (int)fTrueY;
            int iCeilingX = (int)(fTrueX);
            int iCeilingY = (int)(fTrueY);

            if (iFloorX < 0 || iCeilingX < 0 || iFloorX >= src.width || iCeilingX >= src.width || iFloorY < 0 || iCeilingY < 0 || iFloorY >= src.height || iCeilingY >= src.height) continue;

            float fDeltaX = fTrueX - (float)iFloorX;
            float fDeltaY = fTrueY - (float)iFloorY;

            Color clrTopLeft = GetImageColor(src, iFloorX, iFloorY);
            Color clrTopRight = GetImageColor(src, iCeilingX, iFloorY);
            Color clrBottomLeft = GetImageColor(src, iFloorX, iCeilingY);
            Color clrBottomRight = GetImageColor(src, iCeilingX, iCeilingY);

            float fTopRed = (1 - fDeltaX) * clrTopLeft.r + fDeltaX * clrTopRight.r;
            float fTopGreen = (1 - fDeltaX) * clrTopLeft.g + fDeltaX * clrTopRight.g;
            float fTopBlue = (1 - fDeltaX) * clrTopLeft.b + fDeltaX * clrTopRight.b;
            float fTopAlpha = (1 - fDeltaX) * clrTopLeft.a + fDeltaX * clrTopRight.a;

            float fBottomRed = (1 - fDeltaX) * clrBottomLeft.r + fDeltaX * clrBottomRight.r;
            float fBottomGreen = (1 - fDeltaX) * clrBottomLeft.g + fDeltaX * clrBottomRight.g;
            float fBottomBlue = (1 - fDeltaX) * clrBottomLeft.b + fDeltaX * clrBottomRight.b;
            float fBottomAlpha = (1 - fDeltaX) * clrBottomLeft.a + fDeltaX * clrBottomRight.a;

            int iRed = (int)(((1 - fDeltaY) * fTopRed + fDeltaY * fBottomRed));
            int iGreen = (int)(((1 - fDeltaY) * fTopGreen + fDeltaY * fBottomGreen));
            int iBlue = (int)(((1 - fDeltaY) * fTopBlue + fDeltaY * fBottomBlue));
            int iAlpha = (int)(((1 - fDeltaY) * fTopAlpha + fDeltaY * fBottomAlpha));

            if (iRed < 0) iRed = 0;
            if (iRed > 255) iRed = 255;
            if (iGreen < 0) iGreen = 0;
            if (iGreen > 255) iGreen = 255;
            if (iBlue < 0) iBlue = 0;
            if (iBlue > 255) iBlue = 255;
            if (iAlpha < 0) iAlpha = 0;
            if (iAlpha > 255) iAlpha = 255;

            ImageDrawPixel(&out, j, i, (Color){(unsigned char)iRed, (unsigned char)iGreen, (unsigned char)iBlue, (unsigned char)iAlpha});
        }
    }
	return out;
}

void ImageDrawImage(Image* dst, Image src, int posX, int posY, Color tint) {
    Rectangle srcRect = (Rectangle){0, 0, (float)src.width, (float)src.height};
    Rectangle dstRect = (Rectangle){(float)posX, (float)posY, (float)src.width, (float)src.height};
    ImageDraw(dst, src, srcRect, dstRect, tint);
}

#ifdef __cplusplus
}
#endif

#endif  // RAYLIB_IMAGEDRAW_IMPLEMENTATION_ONCE
#endif  // RAYLIB_IMAGEDRAW_IMPLEMENTATION
