# raylib
find_package(raylib QUIET)
if (NOT raylib_FOUND)
    include(FetchContent)
        FetchContent_Declare(
        raylib
        GIT_REPOSITORY https://github.com/raysan5/raylib.git
        GIT_TAG 4.2.0
    )
    FetchContent_GetProperties(raylib)
    if (NOT raylib_POPULATED) # Have we downloaded raylib yet?
        set(FETCHCONTENT_QUIET NO)
        FetchContent_Populate(raylib)
        set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
        set(BUILD_GAMES    OFF CACHE BOOL "" FORCE)
        add_subdirectory(${raylib_SOURCE_DIR} ${raylib_BINARY_DIR})
    endif()
endif()

# raylib-imagedraw-example
add_executable(raylib-imagedraw-example raylib-imagedraw-example.c)
target_link_libraries(raylib-imagedraw-example PUBLIC
    raylib
    raylib_imagedraw
    m
)
set_property(TARGET raylib-imagedraw-example PROPERTY C_STANDARD 99)

# Copy the resources
file(GLOB resources resources/*)
set(test_resources)
list(APPEND test_resources ${resources})
file(COPY ${test_resources} DESTINATION "resources/")
