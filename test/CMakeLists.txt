# raylib-imagedraw-test
add_executable(raylib-imagedraw-test raylib-imagedraw-test.c)
target_compile_options(raylib-imagedraw-test PRIVATE -Wall -Wextra -Wconversion -Wsign-conversion)
target_link_libraries(raylib-imagedraw-test PUBLIC
    raylib
    raylib_imagedraw
    m
)
set_property(TARGET raylib-imagedraw-test PROPERTY C_STANDARD 99)

# Copy the resources
file(GLOB resources resources/*)
set(test_resources)
list(APPEND test_resources ${resources})
file(COPY ${test_resources} DESTINATION "resources/")

# Set up the test
add_test(NAME raylib-imagedraw-test COMMAND raylib-imagedraw-test)
