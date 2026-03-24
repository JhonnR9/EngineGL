//
// Created by jhone on 22/03/2026.
//

#include "my_scene.h"

#include <ctime>
#include "utils/vector2.h"
#include "main/app.h"


MyScene::MyScene(App &app, entt::registry &registry): Scene(app, registry), player_entity() {
}

void MyScene::init() {
    auto yuzuha = registry.create();

    Transform transform;
    transform.position = Vector2(500.f, 400.f);
    transform.scale = Vector2(0.2f, 0.2f);

    Sprite sprite;
    sprite.texture_path = RESOURCE_PATH"/yuzuha.jpg";
    sprite.flipped_x = true;

    registry.emplace<Transform>(yuzuha, transform);
    registry.emplace<Sprite>(yuzuha, sprite);

    player_entity = yuzuha;

    auto &value = registry.ctx().get<int>();
    shape_renderer  = registry.ctx().get<ShapeRenderer *>();

}

void MyScene::update(float delta) {
    if (direction.length() > 0) {
        direction.normalize();
    }
    auto &transform = registry.get<Transform>(player_entity);
    transform.position.x += direction.x * player_speed * delta;
    transform.position.y += direction.y * player_speed * delta;
}

void MyScene::render(SpriteBatch &batch)
{
    shape_renderer->begin();

    // 🔲 RETÂNGULOS
    for (int i = 0; i < 10; i++)
    {
        Rect rect;
        rect.x = 200 + i * 120;
        rect.y = 100;
        rect.width = 80;
        rect.height = 60;

        Color c;
        c.r = 1.0f;
        c.g = i * 0.2f;
        c.b = 0.2f;
        c.a = 1.0f;

        shape_renderer->draw_rect(rect, c);
    }

    // 🔵 CÍRCULOS
    for (int i = 0; i < 10; i++)
    {
        Ellipse e;
        e.cx = 200 + i * 120;
        e.cy = 300;
        e.rx = 40;
        e.ry = 40; // círculo

        Color c;
        c.r = i * 0.2f;
        c.g = 0.5f;
        c.b = 1.0f;
        c.a = 1.0f;

        shape_renderer->draw_eclipse(e, c);
    }

    // 🟣 ELIPSES
    for (int i = 0; i < 10; i++)
    {
        Ellipse e;
        e.cx = 200 + i * 120;
        e.cy = 500;
        e.rx = 60;
        e.ry = 30;

        Color c;
        c.r = 1.0f;
        c.g = 0.3f;
        c.b = i * 0.2f;
        c.a = 1.0f;

        shape_renderer->draw_eclipse(e, c);
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
}
