#ifndef APP_H
#define APP_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <memory>
#include "systems/scene.h"
#include "glm/vec2.hpp"
#include <entt/entt.hpp>

#include "systems/system.h"

class GLFWwindow;

struct WindowConfig {
    bool is_fullscreen = false;
    int windowed_xpos = 100;
    int windowed_ypos = 100;
    int windowed_width = 840;
    int windowed_height = 480;
    bool use_vsync = true;
};

class App {
    WindowConfig config;

    GLFWwindow *window = nullptr;
    std::unique_ptr<Scene> current_scene{nullptr};
    std::vector<std::unique_ptr<System>> systems{};

    std::unique_ptr<entt::registry> registry{nullptr};

    App() = default;

    ~App();

public:
    const float VIRTUAL_WIDTH = 1366.0f;
    const float VIRTUAL_HEIGHT = 768.0f;

    App(const App &) = delete;

    App &operator=(const App &) = delete;

    template<typename T>
    void add_system();

    static App &getInstance() {
        static App instance;
        return instance;
    }

    int get_windowed_width() const { return config.windowed_width; }
    int get_windowed_height() const { return config.windowed_height; }
    GLFWwindow *get_window() const { return window; }

    void setScene(std::unique_ptr<Scene> scene) {
        current_scene = std::move(scene);
        if (current_scene) {
            current_scene->init();
        }
    }

    void init();

    void game_loop();

    void key_callback(GLFWwindow *win, int key, int scancode, int action, int mods);

    static void key_callback_redirect(GLFWwindow *win, int key, int scancode, int action, int mods);

    static void error_callback(int error, const char *description);

    static void framebuffer_size_callback_redirect(GLFWwindow *window, int width, int height);
};

template<typename T>
void App::add_system() {
    static_assert(std::is_base_of_v<System, T>, "T is not inherit from System!");
    systems.emplace_back(std::make_unique<T>(registry));
}



#endif // APP_H
