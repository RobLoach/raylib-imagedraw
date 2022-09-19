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

int main() {
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;
    InitWindow(screenWidth, screenHeight, "[raylib-imagedraw] example");
    SetTargetFPS(60);

    Image image = GenImageColor(screenWidth, 450, RAYWHITE);

    ImageDrawLineHorizontal(&image, 10, 10, 200, RED);
    ImageDrawLineVertical(&image, 10, 10, 200, RED);

    Vector2 points[3] = {
        {100, 100},
        {200, 100},
        {100, 200}
    };
    ImageDrawPoly(&image, points, 3, ORANGE);



    ImageDrawCircleFilled(&image, 200, 200, 40, PURPLE);

    ImageDrawEllipse(&image, 200, 200, 200, 50, BLUE);

    Texture texture = LoadTextureFromImage(image);
    UnloadImage(image);
    //--------------------------------------------------------------------------------------

    while(!WindowShouldClose()) {

        // Update
        //----------------------------------------------------------------------------------

        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
        {
            ClearBackground(RAYWHITE);
            DrawTexture(texture, 0, 0, WHITE);

        }
        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------

    UnloadTexture(texture);
    CloseWindow();
    //--------------------------------------------------------------------------------------

    return 0;
}
