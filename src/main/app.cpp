//
// Created by jhone on 07/08/2025.
//

#include "app.h"

#include <iostream>
#include <ostream>
#include <fstream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/vec3.hpp>

#include "glm/ext/matrix_clip_space.hpp"
#include "systems/render_system.h"

App::~App() {
    glfwDestroyWindow(window);
    glfwTerminate();
}

void App::key_callback_redirect(GLFWwindow *win, const int key, int scancode, const int action, int mods) {
    getInstance().key_callback(win, key, scancode, action, mods);
}


void App::error_callback(const int error, const char *description) {
    std::cerr << "Error: " << error << description << std::endl;
}

void App::framebuffer_size_callback_redirect(GLFWwindow *window, int width, int height) {
    App &app = App::getInstance();

    float targetAspectRatio = app.VIRTUAL_WIDTH / app.VIRTUAL_HEIGHT;

    int viewWidth = width;
    int viewHeight = (int) (viewWidth / targetAspectRatio + 0.5f);

    if (viewHeight > height) {
        viewHeight = height;
        viewWidth = (int) (viewHeight * targetAspectRatio + 0.5f);
    }


    int viewX = (width / 2) - (viewWidth / 2);
    int viewY = (height / 2) - (viewHeight / 2);

    glViewport(viewX, viewY, viewWidth, viewHeight);
    const auto projection = glm::ortho(0.0f, app.VIRTUAL_WIDTH, app.VIRTUAL_HEIGHT, 0.0f, -1.0f, 1.0f);

    app.get_sprite_batch()->set_projection(projection);
}

void App::init() {
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwSetErrorCallback(error_callback);

    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(config.windowed_width, config.windowed_height, "Image Viewer (OpenGL Low-Level)", nullptr,
                              nullptr);
    if (!window) {
        glfwTerminate();
        std::cerr << "Failed to open GLFW window." << std::endl;
        return;
    }

    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback_redirect);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback_redirect);

    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
    }

    glfwSwapInterval(config.use_vsync);

    registry = std::make_unique<entt::registry>();
    batch = std::make_unique<SpriteBatch>(get_windowed_width(), get_windowed_height());
    auto projection = glm::ortho(0.0f, VIRTUAL_WIDTH, VIRTUAL_HEIGHT, 0.0f, -1.0f, 1.0f);
    batch->set_projection(projection);

    systems.emplace_back(std::move(std::make_unique<RenderSystem>(*registry, *batch)));
}

void App::game_loop() {
    float lastTime = glfwGetTime();
    while (!glfwWindowShouldClose(window)) {
        float currentTime = glfwGetTime();
        float deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0.043f, 0.067, 0.090, 1.0f);

        for (auto &system: systems) {
            system->run(deltaTime);
        }
        if (current_scene) {
            current_scene->update(deltaTime);
            current_scene->render(*batch);

        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

void App::key_callback(GLFWwindow *win, const int key, int scancode, const int action, int mods) {

    if (current_scene) {
        current_scene->key_callback(key, scancode, action, mods);
    }
    if ((key == GLFW_KEY_F11 || key == GLFW_KEY_ESCAPE) && action == GLFW_PRESS) {
        if (key == GLFW_KEY_ESCAPE) {
            config.is_fullscreen = false;
        } else {
            config.is_fullscreen = !config.is_fullscreen;
        }

        if (config.is_fullscreen) {
            glfwGetWindowPos(win, &config.windowed_xpos, &config.windowed_ypos);
            glfwGetWindowSize(win, &config.windowed_width, &config.windowed_height);

            GLFWmonitor *monitor = glfwGetPrimaryMonitor();
            const GLFWvidmode *mode = glfwGetVideoMode(monitor);

            glfwSetWindowMonitor(win, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);

            if (config.use_vsync)
                glfwSwapInterval(1);
        } else {
            glfwSetWindowMonitor(win, nullptr, config.windowed_xpos, config.windowed_ypos, config.windowed_width,
                                 config.windowed_height, 0);

            if (config.use_vsync)
                glfwSwapInterval(1);
        }

    }
}
