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

App::~App() {
    glfwDestroyWindow(window);
    glfwTerminate();
}

void App::key_callback_redirect(GLFWwindow *win, const int key, int scancode, const int action, int mods) {
    getInstance().key_callback(win, key, scancode, action, mods);
}


void App::error_callback(const int error, const char *description) {
    std::cerr << "Error: "<< error << description << std::endl;
}

void App::framebuffer_size_callback_redirect(GLFWwindow *window, int width, int height) {
    App& app = App::getInstance();

    float targetAspectRatio = app.VIRTUAL_WIDTH / app.VIRTUAL_HEIGHT;

    int viewWidth = width;
    int viewHeight = (int)(viewWidth / targetAspectRatio + 0.5f);

    if (viewHeight > height) {
        viewHeight = height;
        viewWidth = (int)(viewHeight * targetAspectRatio + 0.5f);
    }


    int viewX = (width / 2) - (viewWidth / 2);
    int viewY = (height / 2) - (viewHeight / 2);

    glViewport(viewX, viewY, viewWidth, viewHeight);

    if (app.current_scene) {

        app.current_scene->resize(app.VIRTUAL_WIDTH, app.VIRTUAL_HEIGHT);
    }
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

    window = glfwCreateWindow(windowed_width, windowed_height, "Image Viewer (OpenGL Low-Level)", nullptr, nullptr);
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

    glfwSwapInterval(0);


}

void App::game_loop() {

    float lastTime = glfwGetTime();
    while (!glfwWindowShouldClose(window)) {
        float currentTime = glfwGetTime();
        float deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        if (current_scene) {
            current_scene->update(deltaTime);
            current_scene->render();
        }


        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

void App::key_callback(GLFWwindow *win, const int key, int scancode, const int action, int mods) {

    if ((key == GLFW_KEY_F11 || key==GLFW_KEY_ESCAPE) && action == GLFW_PRESS) {

        if ( key == GLFW_KEY_ESCAPE) {
            is_fullscreen = false;
        }else {
            is_fullscreen = !is_fullscreen;
        }

        if (is_fullscreen) {
            glfwGetWindowPos(win, &windowed_xpos, &windowed_ypos);
            glfwGetWindowSize(win, &windowed_width, &windowed_height);

            GLFWmonitor* monitor = glfwGetPrimaryMonitor();
            const GLFWvidmode* mode = glfwGetVideoMode(monitor);

            glfwSetWindowMonitor(win, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
        } else {
            glfwSetWindowMonitor(win, nullptr, windowed_xpos, windowed_ypos, windowed_width, windowed_height, 0);
        }
    }
}

