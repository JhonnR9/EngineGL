//
// Created by jhone on 07/08/2025.
//

#ifndef APP_H
#define APP_H
#include "glm/vec2.hpp"

class GLFWwindow;

class App {
    bool is_fullscreen = false;
    int windowed_xpos = 100, windowed_ypos = 100;
    int windowed_width = 640, windowed_height = 480;
    glm::vec2 camera_position = {glm::vec2(0.f)};

    App() = default;
    GLFWwindow* window {nullptr};
public:
    App(const App &) = delete;
    App &operator=(const App &) = delete;

    static App &getInstance() {
        static App instance;
        return instance;
    }

    // we used this for allow members variables in input callback
    static void key_callback_redirect(GLFWwindow *win, int key, int scancode, int action, int mods);
    static void error_callback(int error, const char *description);

    void init();
    void key_callback(GLFWwindow *win, int key, int scancode, int action, int mods);
};

#endif //APP_H
