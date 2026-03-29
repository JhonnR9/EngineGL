#ifndef APP_H
#define APP_H

#include <memory>
#include "scenes/scene.h"
#include "graphics/sprite_batch.h"
#include "systems/system.h"
#include "platforms/window.h"

struct WindowConfig {
    bool is_fullscreen = false;
    int windowed_width = 840;
    int windowed_height = 480;
    bool use_vsync = false;
};

class App {
    WindowConfig config;
    std::unique_ptr<SpriteBatch> batch{nullptr};
    std::unique_ptr<Window> window{nullptr};
    std::unique_ptr<Scene> current_scene{nullptr};
    std::vector<std::unique_ptr<System>> systems{};
    std::unique_ptr<OrthographicCamera> main_camera{nullptr};
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
    entt::registry &get_registry() const {
        return *registry;
    }

    static App &getInstance() {
        static App instance;
        return instance;
    }
    OrthographicCamera* get_main_camera() const {
        return main_camera.get();
    }

    int get_windowed_width() const { return config.windowed_width; }
    int get_windowed_height() const { return config.windowed_height; }

    void setScene(std::unique_ptr<Scene> scene) {
        current_scene = std::move(scene);
        if (current_scene) {
            current_scene->init();
        }
    }

    void init();

    void onKey(int key, int scancode, int action, int mods);

    SpriteBatch* get_sprite_batch() const {
        return batch.get();
    }

    void game_loop();
    static void error_callback(int error, const char *description);
};

template<typename T>
void App::add_system() {
    static_assert(std::is_base_of_v<System, T>, "T is not inherit from System!");
    systems.emplace_back(std::make_unique<T>(registry));
}

#endif // APP_H
