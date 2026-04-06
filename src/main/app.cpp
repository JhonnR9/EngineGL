//
// Created by jhone on 07/08/2025.
//

#include "app.h"
#include "systems/collision_detection_system.h"
#include "systems/overlap_correction_system.h"
#include "systems/render_system.h"
#include <chrono>
#include "platforms/sfml/sfml_window.h"
#include "utils/assets_manager.h"
#include "graphics/orthographic_camera.h"

using Clock = std::chrono::high_resolution_clock;
using TimePoint = std::chrono::time_point<Clock>;

App::App() = default;
App::~App() = default;

void App::init() {
    window = std::make_unique<sfml_window>();

    if (!window->init("My game", 1366, 768, false)) {
        throw std::runtime_error("Failed to create the window!");
    }

    window->set_vsync(false);

    window->set_event_callback([this](const Event& e) {this->on_event(e);});

    auto [width, height] = window->get_size();

    const ResizeEvent event(width, height);
    this->on_event(event);

    registry = std::make_unique<entt::registry>();
    assets_manager = std::make_unique<AssetsManager>();

    main_camera = std::make_unique<OrthographicCamera>(render_size);

    registry->ctx().emplace<OrthographicCamera *>(get_main_camera());
    registry->ctx().emplace<AssetsManager *>(assets_manager.get());

    auto render_system = std::make_unique<RenderSystem>(*registry.get());
    render_system->set_ui_view_size(ui_viewport_size);

    systems.emplace_back(std::move(std::make_unique<CollisionDetectionSystem>(*registry)));
    systems.emplace_back(std::move(std::make_unique<OverlapCorrectionSystem>(*registry)));
    systems.emplace_back(std::move(render_system));
}

void App::on_event(const Event &e) {
    if (current_scene) {
        current_scene->on_event(e);
    }

    if (e.get_type() == EventType::Resize) {
        const auto &ev = static_cast<const ResizeEvent &>(e);
        if (ev.new_width == 0 || ev.new_height == 0) return;

        int view_width = ev.new_width;
        int view_height = ev.new_height;
        int viewX = 0;
        int viewY = 0;

        float w = static_cast<float>(render_size.width);
        float h = static_cast<float>(render_size.height);

        const float target_aspect_ratio = w/ h;
        const float window_aspect_ratio = static_cast<float>(view_width) / static_cast<float>(view_height);


        if (window_aspect_ratio > target_aspect_ratio) {
            view_width = static_cast<int>(ev.new_height * target_aspect_ratio);
            viewX = (ev.new_width - view_width) / 2;
            viewY = 0;
        } else {
            view_height = static_cast<int>(ev.new_width / target_aspect_ratio);
            viewX = 0;
            viewY = (ev.new_height - view_height) / 2;
        }


        glViewport(viewX, viewY, view_width, view_height);
    }

}

void App::game_loop() {
    TimePoint last_time = Clock::now();
    float fps_timer = 0.0f;
    int fps_counter = 0;

    if (current_scene) {
        current_scene->load_assets(assets_manager.get());
        assets_manager->pool_works();
    }

    while (!window->should_close()) {
        window->poll_events();
        if (assets_manager) {
            assets_manager->pool_works();
        }

        TimePoint current_time = Clock::now();
        std::chrono::duration<float> delta = current_time - last_time;
        last_time = current_time;
        const float deltaTime = delta.count();

        fps_counter++;
        fps_timer += deltaTime;
        if (fps_timer >= 1.0f) {
            int fps = fps_counter;
            fps_counter = 0;
            fps_timer -= 1.0f;
            current_fps = fps;
        }

        if (current_scene) {
            current_scene->update(deltaTime);
        }

        for (const auto &system: systems) {
            system->run(deltaTime);
        }

        window->swap_buffers();
    }
}

