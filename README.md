# raylib-imagedraw

Extra image drawing functionality for raylib.

## Usage

``` c
#include "raylib.h"

#define RAYLIB_IMAGEDRAW_IMPLEMENTATION
#include "raylib-imagedraw.h"

int main() {
    const int screenWidth = 800;
    const int screenHeight = 450;
    InitWindow(screenWidth, screenHeight, "[raylib-imagedraw] example");

    Image image = GenImageColor(screenWidth, 450, RAYWHITE);

    ImageDrawLineHorizontal(&image, 10, 10, 200, RED);

    int points[6] = {100, 100, 200, 100, 100, 200};
    ImageDrawPolygon(&image, &points[0], 3, BLACK);

    Texture texture = LoadTextureFromImage(image);
    UnloadImage(image);

    while (!WindowShouldClose()) {
        BeginDrawing();
        {
            ClearBackground(RAYWHITE);
            DrawTexture(texture, 0, 0, WHITE);

        }
        EndDrawing();
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------

    UnloadTexture(texture);
    CloseWindow();
    //--------------------------------------------------------------------------------------

    return 0;
}
```

## API


## License

*raylib-imagedraw* is licensed under an unmodified zlib/libpng license, which is an OSI-certified, BSD-like license that allows static linking with closed source software. Check [LICENSE](LICENSE) for further details.