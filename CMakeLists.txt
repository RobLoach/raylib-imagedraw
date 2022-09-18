cmake_minimum_required(VERSION 3.11)
project (raylib_imagedraw
    VERSION 4.2.1
    DESCRIPTION "raylib_imagedraw: Extra image draw methods for raylib."
    HOMEPAGE_URL "https://github.com/robloach/raylib-imagedraw"
    LANGUAGES C)

# Options
if ("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_CURRENT_SOURCE_DIR}")
    set(RAYLIB_IMAGEDRAW_IS_MAIN TRUE)
else()
    set(RAYLIB_IMAGEDRAW_IS_MAIN FALSE)
endif()
option(RAYLIB_IMAGEDRAW_BUILD_EXAMPLES "Examples" ${RAYLIB_IMAGEDRAW_IS_MAIN})

add_library(raylib_imagedraw INTERFACE)

# Include Directory
target_include_directories(raylib_imagedraw INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/)

# Examples
if(RAYLIB_IMAGEDRAW_BUILD_EXAMPLES)
    add_subdirectory(examples)

    # Testing
    include(CTest)
    enable_testing()
    if(BUILD_TESTING)
        # set(CTEST_CUSTOM_TESTS_IGNORE
        #     pkg-config--static
        # )
        add_subdirectory(test)
    endif()
endif()
