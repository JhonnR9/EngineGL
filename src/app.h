//
// Created by jhone on 07/08/2025.
//

#ifndef APP_H
#define APP_H
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <memory>
#include "scene.h"
#include "glm/vec2.hpp"

class GLFWwindow;

class App {
    bool is_fullscreen = false;
    int windowed_xpos = 100, windowed_ypos = 100;
    int windowed_width = 1366, windowed_height = 768;
    std::unique_ptr<Scene> current_scene{nullptr};


public:
    const float VIRTUAL_WIDTH = 1366.0f;
    const float VIRTUAL_HEIGHT = 768.0f;
    int get_windowed_width() const {
        return windowed_width;
    }

    int get_windowed_height() const {
        return windowed_height;
    }

    void setScene(std::unique_ptr<Scene> scene) {
        current_scene = std::move(scene);
        if (current_scene) {
            current_scene->init();
        }
    }

private:
    App() = default;
    ~App();

    GLFWwindow *window{nullptr};

public:
    GLFWwindow *get_window() const { return window; }

    App(const App &) = delete;

    App &operator=(const App &) = delete;

    static App &getInstance() {
        static App instance;
        return instance;
    }

    static void key_callback_redirect(GLFWwindow *win, int key, int scancode, int action, int mods);

    static void error_callback(int error, const char *description);
    static void framebuffer_size_callback_redirect(GLFWwindow* window, int width, int height);

    void init();

    void game_loop();

    void key_callback(GLFWwindow *win, int key, int scancode, int action, int mods);
};

#endif //APP_H
