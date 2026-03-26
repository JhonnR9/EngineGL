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
    auto yuzuha = registry.create();

    Transform yuzuha_transform;
    yuzuha_transform.position = Vector2(100.f, 100.f);
    yuzuha_transform.scale = Vector2(0.1f, 0.1f);

    Sprite sprite;
    sprite.texture_path = RESOURCE_PATH"/hutao.png";
    sprite.flipped_x = true;

    registry.emplace<Transform>(yuzuha, yuzuha_transform);
    registry.emplace<Sprite>(yuzuha, sprite);

    player_entity = yuzuha;

    shape_renderer = registry.ctx().get<ShapeRenderer *>();
    main_camera = registry.ctx().get<OrthographicCamera *>();
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
    shape_renderer->begin();

    Rect rect;
    rect.x = 200 + 120;
    rect.y = 100;
    rect.width = 500;
    rect.height = 500;

    Color c;
    c.r = 1.0f, c.g = 1.0f, c.b = 1.0f, c.a = 1.0f;

    shape_renderer->draw_rect(rect, c, Vector2::zero(), 0.0f, -1.f);

    Color co;co.r = 1.0f, co.g = .0f, co.b = 1.0f, co.a = 1.0f;

    Ellipse ellipse;
    ellipse.cy = 500, ellipse.cx = 400, ellipse.rx = 200, ellipse.ry = 200;
    shape_renderer->draw_eclipse(ellipse, co, 1.0f);


    Vector2 center(0.0f, 0.0f);
    float radius = 150.0f;
    float thickness = 4.0f;

    Color green;
    green.r = 0.0f;
    green.g = 1.0f;
    green.b = 0.0f;
    green.a = 1.0f;

    const int sides = 5;
    Vector2 points[sides];

    for (int i = 0; i < sides; i++)
    {
        float angle = (2.0f * glm::pi<float>() * i) / sides;

        points[i].x = center.x + cos(angle) * radius;
        points[i].y = center.y + sin(angle) * radius;
    }

    for (int i = 0; i < sides; i++)
    {
        Vector2 a = points[i];
        Vector2 b = points[(i + 1) % sides];

        shape_renderer->draw_line(a, b, thickness, green, 1.0f);
    }

    shape_renderer->end();
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
