#include "raylib.h"
#include "raylib-assert.h"

#define RAYLIB_IMAGEDRAW_IMPLEMENTATION
#include "raylib-imagedraw.h"

int main(int argc, char *argv[]) {
    // Initialization
    SetTraceLogLevel(LOG_ALL);
    TraceLog(LOG_INFO, "================================");
    TraceLog(LOG_INFO, "raylib-imagedraw-test");
    TraceLog(LOG_INFO, "================================");

    // Make sure we're running in the correct directory.
    Assert(argc > 0);
    const char* dir = GetDirectoryPath(argv[0]);
    Assert(ChangeDirectory(dir));

    Image image = GenImageColor(100, 100, RED);
    ImageDrawLineHorizontal(&image, 10, 10, 50, GREEN);
    ImageDrawLineVertical(&image, 10, 10, 50, GREEN);
    Vector2 points[3] = {
        {10, 60},
        {80, 65},
        {20, 80}
    };
    ImageDrawPoly(&image, points, 3, ORANGE);
    ImageDrawPolyLines(&image, points, 3, BLACK);
    ImageDrawEllipse(&image, 30, 30, 15, 20, BLUE);
    ImageDrawEllipseLines(&image, 30, 30, 15, 20, BLACK);
    ExportImage(image, "out.png");

    Image expected = LoadImage("resources/expected.png");
    AssertImage(expected);
    AssertImageSame(expected, image);

    UnloadImage(image);
    UnloadImage(expected);

    TraceLog(LOG_INFO, "================================");
    TraceLog(LOG_INFO, "raylib-imagedraw tests succesful");
    TraceLog(LOG_INFO, "================================");

    return 0;
}
