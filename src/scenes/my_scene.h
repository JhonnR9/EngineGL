//
// Created by jhone on 22/03/2026.
//

#ifndef MY_SCENE_H
#define MY_SCENE_H
#include <memory>

#include "scene.h"
#include "../graphics/sprite_batch.h"
#include "graphics/texture_2d.h"
#include "entt/entt.hpp"
#include "graphics/shape_renderer.h"


class MyScene final : public Scene {
    entt::entity player_entity;
    float player_speed{400};
    Vector2 direction;
    bool right{false}, left{false}, up{false}, down{false};
    OrthographicCamera* main_camera{nullptr};
    Vector2 position_cache{0.0f,0.0f};
    bool zoom_in = false;
    bool zoom_out = false;

    float zoom = 1.0f;
    float zoom_speed = 1.5f;
    void spawn_box(entt::registry& registry, Vector2 pos, float w, float h, Color color, bool is_static = true);

public:
    MyScene(App &app, entt::registry &registry);

    void init() override;

    void update(float delta) override;

    void render(SpriteBatch &batch) override;

    void key_callback(int key, int scancode, int action, int mods) override;
};


#endif //MY_SCENE_H
