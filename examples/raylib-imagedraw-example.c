/*******************************************************************************************
*
*   [raylib-imagedraw] example - Extra image draw methods for raylib.
*
*   This example has been created using raylib 4.2 (www.raylib.com)
*   raylib is licensed under an unmodified zlib/libpng license (View raylib.h for details)
*
*   Example by Rob Loach (@RobLoach)
*
*   Copyright (c) 2021 Rob Loach (@RobLoach)
*
********************************************************************************************/

#include "raylib.h"

#define RAYLIB_IMAGEDRAW_IMPLEMENTATION
#include "raylib-imagedraw.h"

int main(int argc, char *argv[]) {
    // Initialization
    //--------------------------------------------------------------------------------------

    // Make sure we're running in the correct directory.
    const char* dir = GetDirectoryPath(argv[0]);
    ChangeDirectory(dir);

    const int screenWidth = 800;
    const int screenHeight = 450;
    const int fontSize = 10;
    InitWindow(screenWidth, screenHeight, "[raylib-imagedraw] example");
    SetTargetFPS(10);

    // Create an image.
    Image screen = GenImageColor(screenWidth, screenHeight, RAYWHITE);

    // ImageDrawLine
    ImageDrawText(&screen, "Lines", 10, 10, fontSize, BLACK);
    ImageDrawLineHorizontal(&screen, 10, 20, 20, RED);
    ImageDrawLineVertical(&screen, 10, 20, 20, RED);

    // ImageDrawPoly
    ImageDrawText(&screen, "Poly", 10, 50, fontSize, BLACK);
    Vector2 points[3] = {
        {10, 60},
        {80, 65},
        {20, 80}
    };
    ImageDrawPoly(&screen, points, 3, ORANGE);
    ImageDrawPolyLines(&screen, points, 3, BLACK);

    // ImageDrawEllipse
    ImageDrawText(&screen, "Ellipse", 10, 100, fontSize, BLACK);
    ImageDrawEllipse(&screen, 30, 130, 15, 20, BLUE);
    ImageDrawEllipseLines(&screen, 30, 130, 15, 20, BLACK);

    // ImageRotate
    ImageDrawText(&screen, "Rotate", 100, 10, fontSize, BLACK);
    Image carl = LoadImage("resources/carlsagan.png");
    ImageDrawImage(&screen, carl, 100, 20, WHITE);
    TraceLog(LOG_INFO, "Size: %ix%i", carl.width, carl.height);

    Texture screenTexture = LoadTextureFromImage(screen);
    float rotation = 0;
    //--------------------------------------------------------------------------------------

    while(!WindowShouldClose()) {

        // Update
        //----------------------------------------------------------------------------------
        rotation += 4;
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        Image carlRotated = ImageRotate(carl, rotation);
        BeginDrawing();
        {
            ClearBackground(RAYWHITE);
            DrawTexture(screenTexture, 0, 0, WHITE);

            DrawImage(carlRotated, 100 + carl.width / 2 - carlRotated.width / 2, 20 + carl.height + 50 + carl.height / 2 - carlRotated.height / 2);
        }
        EndDrawing();
        UnloadImage(carlRotated);
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------

    UnloadImage(screen);
    UnloadImage(carl);
    UnloadTexture(screenTexture);

    CloseWindow();
    //--------------------------------------------------------------------------------------

    return 0;
}
