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

void App::key_callback_redirect(GLFWwindow *win, const int key, int scancode, const int action, int mods) {
    getInstance().key_callback(win, key, scancode, action, mods);
}

void App::error_callback(const int error, const char *description) {
    std::cerr << "Error: "<< error << description << std::endl;
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

    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
    }

    glfwSwapInterval(1);

}

void App::loop() {

}

void App::key_callback(GLFWwindow *win, const int key, int scancode, const int action, int mods) {
    if (key == GLFW_KEY_LEFT && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
        camera_position.x += 0.01f;
    }else if (key == GLFW_KEY_RIGHT && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
        camera_position.x -= 0.01f;
    }else if (key == GLFW_KEY_DOWN && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
        camera_position.y += 0.01f;
    }else if (key == GLFW_KEY_UP && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
        camera_position.y -= 0.01f;
    }

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

