//
// Created by jhone on 22/03/2026.
//

#include "my_scene.h"
#include <iostream>
#include <numbers>
#include <cmath>
#include <ctime>
#include "utils/vector2.h"
#include "main/app.h"


MyScene::MyScene(App &app, entt::registry &registry): Scene(app, registry), player_entity() {
}

void MyScene::init() {
    main_camera = registry.ctx().get<OrthographicCamera *>();

    auto player = registry.create();
    Transform player_transform;
    Color player_color;
    Rect player_rect;
    ZIndex player_z_index;

    player_transform.position = Vector2(100.f, 100.f);
    player_rect.width = 150.f;
    player_rect.height = 150.f;
    player_z_index.value = 1;

    registry.emplace<Transform>(player, player_transform);
    registry.emplace<Rect>(player, player_rect);
    registry.emplace<ZIndex>(player, player_z_index);
    registry.emplace<Color>(player, player_color);


    auto wall = registry.create();
    Transform box_transform;
    Rect box_rect;
    ZIndex box_z_index;
    Color box_color;

    box_z_index.value = .0f;
    box_color.r = 0.0f;

    box_rect.width = 500.f;
    box_rect.height = 100.f;

    registry.emplace<Rect>(wall, box_rect);
    registry.emplace<ZIndex>(wall, box_z_index);
    registry.emplace<Transform>(wall, box_transform);
    registry.emplace<Color>(wall, box_color);

    player_entity = player;

}

void MyScene::update(float delta) {
    auto &transform = registry.get<Transform>(player_entity);
    float smooth = 2.0f;
    float t = 1.0f - std::exp(-smooth * delta);
    if (direction.length() > 0) {
        direction.normalize();

        transform.position.x += direction.x * player_speed * delta;
        transform.position.y += direction.y * player_speed * delta;


        if (transform.position != position_cache) {
        }
        position_cache = transform.position;
    }
    main_camera->set_position(
        Vector2::lerp(main_camera->get_position(), transform.position, t)
    );

    if (zoom_in) {
        zoom -= zoom_speed * delta;
    }
    if (zoom_out) {
        zoom += zoom_speed * delta;
    }

    if (zoom < 0.2f) zoom = 0.2f;
    if (zoom > 5.0f) zoom = 5.0f;


    main_camera->set_zoom(zoom);
}

void MyScene::render(SpriteBatch &batch) {

}


void MyScene::key_callback(int key, int scancode, int action, int mods) {
    bool pressed = action != 0;

    if (key == 'W') up = pressed;
    if (key == 'S') down = pressed;
    if (key == 'A') left = pressed;
    if (key == 'D') right = pressed;

    direction.x = (right ? 1.f : 0.f) - (left ? 1.f : 0.f);
    direction.y = (down ? 1.f : 0.f) - (up ? 1.f : 0.f);

    if (key == GLFW_KEY_UP) zoom_in = pressed;
    if (key == GLFW_KEY_DOWN) zoom_out = pressed;
}
