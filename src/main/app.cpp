//
// Created by jhone on 07/08/2025.
//

#include "app.h"
#include <iostream>
#include <fstream>
#include "systems/collision_detection_system.h"
#include "systems/overlap_correction_system.h"
#include "systems/render_system.h"
#include <chrono>

#include "platforms/win_32_window.h"

using Clock = std::chrono::high_resolution_clock;
using TimePoint = std::chrono::time_point<Clock>;

App::~App() {

}

void App::error_callback(const int error, const char *description) {
    std::cerr << "Error: " << error << description << std::endl;
}

void App::init() {
    window = std::make_unique<Win32Window>();
    if (!window->init("My game", 1366, 768, false)) {
        throw std::runtime_error("Failed to create the window!");
    }
    window->setKeyCallback([this](int key, int scancode, int action, int mods) {
           this->onKey(key, scancode, action, mods);
       });


    registry = std::make_unique<entt::registry>();
    main_camera = std::make_unique<OrthographicCamera>(VIRTUAL_WIDTH, VIRTUAL_HEIGHT);
    batch = std::make_unique<SpriteBatch>(get_main_camera());

    registry->ctx().emplace<SpriteBatch*>(batch.get());
    registry->ctx().emplace<OrthographicCamera*>(get_main_camera());

    systems.emplace_back(std::move(std::make_unique<CollisionDetectionSystem>(*registry)));
    systems.emplace_back(std::move(std::make_unique<OverlapCorrectionSystem>(*registry)));
    systems.emplace_back(std::move(std::make_unique<RenderSystem>(*registry, *batch)));

}

void App::onKey(int key, int scancode, int action, int mods) {
}

void App::game_loop() {

    TimePoint lastTime = Clock::now();
    while (!window->shouldClose()) {
        TimePoint currentTime = Clock::now();
        std::chrono::duration<float> delta = currentTime - lastTime;
        lastTime = currentTime;

        float deltaTime = delta.count();


        if (current_scene) {
            current_scene->update(deltaTime);
            current_scene->render(*batch);

        }

        for (auto &system: systems) {
            system->run(deltaTime);
        }


        window->update();
    }
}

