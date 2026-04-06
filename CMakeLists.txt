cmake_minimum_required(VERSION 3.31)
include(FetchContent)

project(EngineGL VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(GLM_BUILD_LIBRARY ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)



find_package(OpenGL REQUIRED)
find_package(tinyxml2 REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(Freetype REQUIRED)
find_package(Stb REQUIRED)
find_package(EnTT CONFIG REQUIRED)


set(RESOURCE_DIR "${CMAKE_SOURCE_DIR}/resources")
add_definitions(-DRESOURCE_PATH="${RESOURCE_DIR}")
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
        src/platforms/windows/win_32_window.cpp
        src/utils/tmx_reader.cpp
        src/utils/tsx_reader.cpp
        src/platforms/sfml/sfml_window.cpp
        src/platforms/sfml/sfml_window.h
        src/utils/assets_manager.cpp
        src/utils/assets_manager.h

)

add_library(glad STATIC src/vendor/glad/glad.c)

target_include_directories(glad PUBLIC src/vendor/glad)

target_include_directories(EngineGL PRIVATE src/)

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    message(STATUS "Configurando flags de otimização agressiva para Release")

    if(MSVC)
        target_compile_options(EngineGL PRIVATE /O2 /Oi /Ot /GT /GL /DNDEBUG)
        target_link_options(EngineGL PRIVATE /LTCG /INCREMENTAL:NO /OPT:REF /OPT:ICF)
    else()

        target_compile_options(EngineGL PRIVATE
                -O3
                -flto
                -ffast-math
                -DNDEBUG
        )
        target_link_options(EngineGL PRIVATE -flto)
    endif()

    if(NOT CMAKE_BUILD_TYPE)
        set(CMAKE_BUILD_TYPE Release)
    endif()


    include(CheckIPOSupported)
    check_ipo_supported(RESULT lto_supported OUTPUT error)
    if(lto_supported)
        set_target_properties(EngineGL PROPERTIES INTERPROCEDURAL_OPTIMIZATION TRUE)
    endif()
endif()


target_link_libraries(EngineGL
        PRIVATE
        OpenGL::GL
        glad
        tinyxml2::tinyxml2
        Freetype::Freetype
        EnTT::EnTT
        glm::glm
)

add_subdirectory(src/platforms/sfml)