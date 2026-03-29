cmake_minimum_required(VERSION 3.31)
include(FetchContent)

project(EngineGL VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(GLM_BUILD_LIBRARY ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)


find_package(OpenGL REQUIRED)
set(RESOURCE_DIR "${CMAKE_SOURCE_DIR}/resources")
add_definitions(-DRESOURCE_PATH="${RESOURCE_DIR}")


FetchContent_Declare(
        freetype
        GIT_REPOSITORY https://gitlab.freedesktop.org/freetype/freetype.git
        GIT_TAG VER-2-14-3
)

FetchContent_MakeAvailable(freetype)
add_compile_definitions(UNICODE _UNICODE)

add_executable(EngineGL
        src/main/main.cpp
        src/main/app.cpp
        src/graphics/shader.cpp
        src/graphics/texture_2d.cpp
        src/graphics/sprite_batch.cpp
        src/scenes/scene.cpp
        src/scenes/my_scene.cpp
        src/systems/render_system.cpp
        src/graphics/orthographic_camera.cpp
        src/systems/collision_detection_system.cpp
        src/systems/overlap_correction_system.cpp
        src/graphics/font.cpp
        src/platforms/window.cpp
        src/platforms/win_32_window.cpp
        src/platforms/win_32_window.h

)

target_include_directories(EngineGL PRIVATE "src/vendor")
target_include_directories(EngineGL PRIVATE src/vendor/glm)
target_compile_options(EngineGL PRIVATE -Wno-unused-value)

add_library(glad STATIC src/vendor/glad/glad.c)
target_include_directories(glad PUBLIC src/vendor/glad)
target_include_directories(EngineGL PRIVATE src)

target_link_libraries(EngineGL
        PRIVATE
        OpenGL::GL
        glad
        freetype
)

