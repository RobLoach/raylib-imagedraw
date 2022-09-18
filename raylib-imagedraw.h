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

void ImageDrawLineHorizontal(Image* dst, int posX, int posY, int width, Color color);
void ImageDrawLineVertical(Image* dst, int posX, int posY, int width, Color color);
void ImageDrawPolygonLines(Image* dst, Vector2* points, int pointCount, Color color);

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
    Rectangle rect = {posX, posY, width, 1};
    ImageDrawRectangleRec(dst, rect, color);
}

void ImageDrawLineVertical(Image* dst, int posX, int posY, int height, Color color) {
    for (int y = 0; y < height; y++) {
        ImageDrawPixel(dst, posX, posY + y, color);
    }
}

void ImageDrawPolygonLines(Image* dst, Vector2* points, int pointCount, Color color) {
    for (int i = 0; i < pointCount - 1; i++) {
        ImageDrawLineV(dst, points[i], points[i + 1], color);
    }
    ImageDrawLineV(dst, points[0], points[pointCount - 1], color);
}

#ifdef __cplusplus
}
#endif

#endif  // RAYLIB_IMAGEDRAW_IMPLEMENTATION_ONCE
#endif  // RAYLIB_IMAGEDRAW_IMPLEMENTATION
