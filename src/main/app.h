#ifndef APP_H
#define APP_H

#include <memory>
#include "scenes/scene.h"
#include "graphics/sprite_batch.h"
#include "systems/system.h"
#include "platforms/window.h"



class App {
    std::unique_ptr<SpriteBatch> batch{nullptr};
    std::unique_ptr<Window> window{nullptr};
    std::unique_ptr<Scene> current_scene{nullptr};
    std::vector<std::unique_ptr<System>> systems{};
    std::unique_ptr<OrthographicCamera> main_camera{nullptr};
    std::unique_ptr<entt::registry> registry{nullptr};



public:
    const float virtual_width {1366.0f};
    const float virtual_height { 768.0f};
    App() = default;
    App(const App &) = delete;
    App &operator=(const App &) = delete;

    template<typename T>
    void add_system();

    entt::registry &get_registry() const {
        return *registry;
    }

    OrthographicCamera* get_main_camera() const {
        return main_camera.get();
    }

    void setScene(std::unique_ptr<Scene> scene) {
        current_scene = std::move(scene);
        if (current_scene) {
            current_scene->init();
        }
    }

    void init();
    void on_event(const Event &e);

    SpriteBatch* get_sprite_batch() const {
        return batch.get();
    }

    void game_loop();

};

template<typename T>
void App::add_system() {
    static_assert(std::is_base_of_v<System, T>, "T is not inherit from System!");
    systems.emplace_back(std::make_unique<T>(registry));
}

#endif // APP_H
