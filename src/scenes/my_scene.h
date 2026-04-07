//
// Created by jhone on 22/03/2026.
//

#ifndef MY_SCENE_H
#define MY_SCENE_H
#include <memory>

#include "scene.h"
#include "graphics/sprite_batch.h"
#include "../resource_types/texture_2d.h"
#include "entt/entt.hpp"
#include "utils/assets_manager.h"

class MyScene final : public Scene {
    entt::entity player_entity;
    entt::entity text_entity;

    float player_speed{150};
    Vector2 direction;
    bool right{false}, left{false}, up{false}, down{false};
    OrthographicCamera* main_camera{nullptr};
    Vector2 position_cache{0.0f,0.0f};

public:
    void load_assets(AssetsManager *manager) override;

private:
    float zoom = 1.0f;
    float zoom_speed = 0.2f;
    float last_move_time{0.0f};

    std::shared_ptr<Font> font{nullptr};
    std::unique_ptr<Texture2D> texture{nullptr};
public:
    MyScene(App &app, entt::registry &registry);

    void init() override;

    void update(float delta) override;

    void on_event(const Event &e) override;
};


#endif //MY_SCENE_H
