cmake_minimum_required(VERSION 3.31)
include(FetchContent)

project(EngineGL VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 17)
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
        src/main.cpp
        src/app.cpp
        src/graphics/shader.cpp
        src/graphics/texture_2d.cpp
        src/graphics/material.cpp
        src/graphics/renderer.cpp
        src/sprite.cpp
        src/sprite.h
)

target_include_directories(EngineGL PRIVATE "src/vendor")

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

