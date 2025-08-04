cmake_minimum_required(VERSION 3.31)

project(EngineGL VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


set(glad_SOURCE_DIR "src/vendor/glad")
set(glm_SOURCE_DIR "src/vendor/glm")


find_package(OpenGL REQUIRED)


include(FetchContent)
FetchContent_Declare(
        glfw
        GIT_REPOSITORY https://github.com/glfw/glfw.git
        GIT_TAG        3.3.8
)
FetchContent_MakeAvailable(glfw)


set(GLM_BUILD_LIBRARY OFF)
add_subdirectory(${glm_SOURCE_DIR})


add_executable(EngineGL
        src/main.cpp
)


target_include_directories(EngineGL PRIVATE ${glad_SOURCE_DIR})
target_sources(EngineGL PRIVATE ${glad_SOURCE_DIR}/glad.c)


target_link_libraries(EngineGL
        PRIVATE
        glfw
        OpenGL::GL
        glm::glm
)
