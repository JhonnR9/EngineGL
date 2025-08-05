cmake_minimum_required(VERSION 3.31)
include(FetchContent)

project(EngineGL VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(GLM_BUILD_LIBRARY OFF)


find_package(OpenGL REQUIRED)
add_subdirectory(src/vendor/glm)
set(RESOURCE_DIR "${CMAKE_SOURCE_DIR}/resources")
add_definitions(-DRESOURCE_PATH="${RESOURCE_DIR}")

FetchContent_Declare(
        glfw
        GIT_REPOSITORY https://github.com/glfw/glfw.git
        GIT_TAG        3.3.8
)
FetchContent_MakeAvailable(glfw)


add_executable(EngineGL
        src/main.cpp
        src/Shader.cpp
        src/Shader.h
        src/Mesh.cpp
        src/Mesh.h
)

target_include_directories(EngineGL PRIVATE "src/vendor")
target_sources(EngineGL PRIVATE src/vendor//glad/glad.c)

target_link_libraries(EngineGL
        PRIVATE
        glfw
        OpenGL::GL
        glm::glm
)
