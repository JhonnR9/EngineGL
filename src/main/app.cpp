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

void App::init() {
    window = std::make_unique<Win32Window>();
    window->useVsync(true);

    if (!window->init("My game", 800, 600, false)) {
        throw std::runtime_error("Failed to create the window!");
    }
    window->setKeyCallback([this](int key, int scancode, int action, int mods) {
        this->on_key(key, scancode, action, mods);
    });
    window->setResizeCallback([this](int width, int height) { this->on_resize(width, height); });
    window->setMouseButtonCallback([this](int button, int action, int x, int y) {
        this->on_mouse_button(button, action, x, y);
    });
    window->setMouseMoveCallback([this](int x, int y) { this->on_mouse_move(x, y); });
    window->setMouseWheelCallback([this](int wheel) { this->on_mouse_wheel(wheel); });

    auto currentSize = window->getSize();
    this->on_resize(currentSize.width, currentSize.height);

    registry = std::make_unique<entt::registry>();
    main_camera = std::make_unique<OrthographicCamera>(VIRTUAL_WIDTH, VIRTUAL_HEIGHT);
    batch = std::make_unique<SpriteBatch>(get_main_camera());

    registry->ctx().emplace<SpriteBatch *>(batch.get());
    registry->ctx().emplace<OrthographicCamera *>(get_main_camera());

    systems.emplace_back(std::move(std::make_unique<CollisionDetectionSystem>(*registry)));
    systems.emplace_back(std::move(std::make_unique<OverlapCorrectionSystem>(*registry)));
    systems.emplace_back(std::move(std::make_unique<RenderSystem>(*registry, *batch)));
}

void App::on_key(int key, int scancode, int action, int mods) {
    if (current_scene) {
        current_scene->on_key_event(key, scancode, action, mods);
    }
}

void App::on_resize(int width, int height) {
    if (width == 0 || height == 0) return;

    float targetAspectRatio = (float) VIRTUAL_WIDTH / (float) VIRTUAL_HEIGHT;
    float windowAspectRatio = (float) width / (float) height;

    int viewWidth = width;
    int viewHeight = height;
    int viewX = 0;
    int viewY = 0;


    if (windowAspectRatio > targetAspectRatio) {
        viewWidth = static_cast<int>(height * targetAspectRatio);
        viewX = (width - viewWidth) / 2;
    } else {
        viewHeight = static_cast<int>(width / targetAspectRatio);
        viewY = (height - viewHeight) / 2;
    }

    glViewport(viewX, viewY, viewWidth, viewHeight);
}


void App::on_mouse_button(int button, int action, int x, int y) {
    if (current_scene) {
        current_scene->on_mouse_button_event(button, action, x, y);
    }
}

void App::on_mouse_move(int x, int y) {
    if (current_scene) {
        current_scene->on_mouse_move_event(x, y);
    }
}

void App::on_mouse_wheel(int delta) {
    if (current_scene) {
        current_scene->on_mouse_wheel_event(delta);
    }
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
