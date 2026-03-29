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


void MyScene::spawn_box(entt::registry &registry, Vector2 pos, float w, float h, Color color, bool is_static) {
    auto entity = registry.create();

    Transform transform;
    transform.position = pos;

    Rect rect;
    rect.width = w;
    rect.height = h;

    BoxCollider2D collider;
    collider.width = w;
    collider.height = h;
    collider.is_static = is_static;
    collider.is_trigger = false;

    ZIndex z;
    z.value = 0.0f;

    registry.emplace<Transform>(entity, transform);
    registry.emplace<Rect>(entity, rect);
    registry.emplace<BoxCollider2D>(entity, collider);
    registry.emplace<Color>(entity, color);
    registry.emplace<ZIndex>(entity, z);
}

MyScene::MyScene(App &app, entt::registry &registry): Scene(app, registry), player_entity() {
}

void MyScene::init() {
    if (registry.ctx().contains<OrthographicCamera*>()) {
        main_camera = registry.ctx().get<OrthographicCamera*>();
    }

    std::srand(static_cast<unsigned>(std::time(nullptr)));

    // --- PLAYER ---
    auto player = registry.create();
    player_entity = player;

    Transform player_transform{};
    player_transform.position = Vector2(0.f, 0.f);
    player_transform.scale = Vector2(.2f, .2f);

    BoxCollider2D player_collider{};
    player_collider.width = 100.f;
    player_collider.height = 100.f;


    Sprite player_sprite{};
    player_sprite.texture_path = RESOURCE_PATH"/yuzuha.jpg";

    registry.emplace<Transform>(player, player_transform);
    registry.emplace<Color>(player, Color{1, 0, 0, 1});
    registry.emplace<BoxCollider2D>(player, player_collider);
    registry.emplace<Sprite>(player, player_sprite);
    registry.emplace<ZIndex>(player, ZIndex{0.0f});

    // --- TEXT ---
    auto ui = registry.create();
    Transform label_transform;
    label_transform.position = Vector2(0.f, 0.f);
    font = std::make_shared<Font>(RESOURCE_PATH"/Coolvetica Rg.otf", 128);

    Label label{};
    label.font = font;
    label.text = "Texto pro 123";
    label.font_size = 128;
    label.color=Color{1, 1, 0, 1};

    registry.emplace<Label>(ui, label);
    registry.emplace<Transform>(ui, label_transform);
    registry.emplace<ZIndex>(ui, ZIndex{1.f});

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
