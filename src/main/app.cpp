//
// Created by jhone on 07/08/2025.
//

#include "app.h"
#include "systems/collision_detection_system.h"
#include "systems/overlap_correction_system.h"
#include "systems/render_system.h"
#include <chrono>
#include "platforms/win_32_window.h"

using Clock = std::chrono::high_resolution_clock;
using TimePoint = std::chrono::time_point<Clock>;

void App::init() {
    window = std::make_unique<Win32Window>();
    window->useVsync(true);

    if (!window->init("My game", 800, 600, false)) {
        throw std::runtime_error("Failed to create the window!");
    }

    window->setEventCallback([this](const Event& e) {this->on_event(e);});

    auto [width, height] = window->getSize();

    const ResizeEvent event(width, height);
    this->on_event(event);

    registry = std::make_unique<entt::registry>();
    main_camera = std::make_unique<OrthographicCamera>(virtual_width, virtual_height);
    batch = std::make_unique<SpriteBatch>(get_main_camera());

    registry->ctx().emplace<SpriteBatch *>(batch.get());
    registry->ctx().emplace<OrthographicCamera *>(get_main_camera());

    systems.emplace_back(std::move(std::make_unique<CollisionDetectionSystem>(*registry)));
    systems.emplace_back(std::move(std::make_unique<OverlapCorrectionSystem>(*registry)));
    systems.emplace_back(std::move(std::make_unique<RenderSystem>(*registry, *batch)));
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

        const float target_aspect_ratio = static_cast<float>(virtual_width) / static_cast<float>(virtual_height);
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
    while (!window->shouldClose()) {
        TimePoint current_time = Clock::now();
        std::chrono::duration<float> delta = current_time - last_time;
        last_time = current_time;

        const float deltaTime = delta.count();

        if (current_scene) {
            current_scene->update(deltaTime);
            current_scene->render(*batch);
        }

        for (const auto &system: systems) {
            system->run(deltaTime);
        }

        window->update();
    }
}
