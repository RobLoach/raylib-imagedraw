#include <assert.h>

#include "raylib.h"

#define RAYLIB_IMAGEDRAW_IMPLEMENTATION
#include "raylib-imagedraw.h"

int main(int argc, char *argv[]) {
    // Initialization
    SetTraceLogLevel(LOG_ALL);
    TraceLog(LOG_INFO, "================================");
    TraceLog(LOG_INFO, "raylib-imagedraw-test");
    TraceLog(LOG_INFO, "================================");

    // Make sure we're running in the correct directory.
    assert(argc > 0);
    const char* dir = GetDirectoryPath(argv[0]);
    assert(ChangeDirectory(dir));


    
    TraceLog(LOG_INFO, "================================");
    TraceLog(LOG_INFO, "raylib-aseprite tests succesful");
    TraceLog(LOG_INFO, "================================");

    return 0;
}
