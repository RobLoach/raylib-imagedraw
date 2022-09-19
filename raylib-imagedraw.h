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
RLAPI void ImageDrawCircleFilled(Image* dst, int centerX, int centerY, int radius, Color color);
RLAPI void ImageDrawEllipse(Image* dst, int centerX, int centerY, int radiusX, int radiusY, Color color);
RLAPI void ImageDrawEllipseLines(Image* dst, int centerX, int centerY, int radiusX, int radiusY, Color color);

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

#ifndef RAYLIB_IMAGEDRAW_NO_MATH
#include <math.h>
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
            miny = points[i].y;
        }
        if (maxy < points[i].y) {
            maxy = points[i].y;
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
                nodes[count] = (int)(x0 + (y - y0) / (y1 - y0) * (x1 - x0));
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

void ImageDrawCircleFilled(Image* dst, int centerX, int centerY, int radius, Color color) {
    int x = 0;
    int y = radius;
    int d = 3 - 2 * radius;

    while (y >= x) {
        ImageDrawLineHorizontal(dst, centerX - x, centerY + y, x * 2, color);
        ImageDrawLineHorizontal(dst, centerX - x, centerY - y, x * 2, color);
        ImageDrawLineHorizontal(dst, centerX - y, centerY + x, y * 2, color);
        ImageDrawLineHorizontal(dst, centerX - y, centerY - x, y * 2, color);
        x++;

        if (d > 0) {
            y--;
            d = d + 4 * (x - y) + 10;
        } else {
            d = d + 4 * x + 6;
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

#ifdef __cplusplus
}
#endif

#endif  // RAYLIB_IMAGEDRAW_IMPLEMENTATION_ONCE
#endif  // RAYLIB_IMAGEDRAW_IMPLEMENTATION
