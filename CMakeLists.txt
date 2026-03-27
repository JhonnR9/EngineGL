cmake_minimum_required(VERSION 3.31)
include(FetchContent)

project(EngineGL VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(GLM_BUILD_LIBRARY OFF)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)


find_package(OpenGL REQUIRED)
add_subdirectory(src/vendor/glm)
set(RESOURCE_DIR "${CMAKE_SOURCE_DIR}/resources")
add_definitions(-DRESOURCE_PATH="${RESOURCE_DIR}")

FetchContent_Declare(
        glfw
        GIT_REPOSITORY https://github.com/glfw/glfw.git
        GIT_TAG        3.3.8
)

set(BUILD_SHARED_LIBS ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(glfw)


add_executable(EngineGL
        src/main/main.cpp
        src/main/app.cpp
        src/graphics/shader.cpp
        src/graphics/texture_2d.cpp
        src/graphics/sprite_batch.cpp
        src/scenes/scene.cpp
        src/scenes/my_scene.cpp
        src/systems/system.h
        src/systems/render_system.cpp
        src/systems/render_system.h
        src/graphics/shape_renderer.cpp
        src/graphics/shape_renderer.h
        src/graphics/orthographic_camera.cpp
        src/graphics/orthographic_camera.h
        src/systems/shape_renderer_system.cpp
        src/systems/shape_renderer_system.h
        src/systems/collision_detection_system.cpp
        src/systems/collision_detection_system.h
        src/systems/overlap_correction_system.cpp
        src/systems/overlap_correction_system.h
)

target_include_directories(EngineGL PRIVATE "src/vendor")
target_compile_options(EngineGL PRIVATE -Wno-unused-value)

add_library(glad STATIC src/vendor/glad/glad.c)
target_include_directories(glad PUBLIC src/vendor/glad)
target_include_directories(EngineGL PRIVATE src)

target_link_libraries(EngineGL
        PRIVATE
        glfw
        OpenGL::GL
        glm::glm
        glad
)

