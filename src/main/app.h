#ifndef APP_H
#define APP_H

#include <memory>
#include "scenes/scene.h"
#include "graphics/sprite_batch.h"
#include "systems/system.h"
#include "platforms/window.h"
#include "utils/assets_manager.h"

class AssetsManager;
class OrthographicCamera;

class App {
    std::unique_ptr<Window> window{nullptr};
    std::unique_ptr<Scene> current_scene{nullptr};
    std::vector<std::unique_ptr<System>> systems{};
    std::unique_ptr<OrthographicCamera> main_camera{nullptr};
    std::unique_ptr<entt::registry> registry{nullptr};
    std::unique_ptr<AssetsManager> assets_manager{nullptr};

    float current_fps{0.0f};
public:
    Size render_size{800, 600};
    App();
    ~App();

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
            current_scene->load_assets(assets_manager.get());
            assets_manager->pool_works();
            current_scene->init();
        }
    }

    Size get_render_size() const {
        return render_size;
    }
    const float &get_current_fps() {
        return current_fps;
    }

    void init();
    void on_event(const Event &e);

    void game_loop();

};

template<typename T>
void App::add_system() {
    static_assert(std::is_base_of_v<System, T>, "T is not inherit from System!");
    systems.emplace_back(std::make_unique<T>(registry));
}

#endif // APP_H
