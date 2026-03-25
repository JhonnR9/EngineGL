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

    Transform yuzuha_transform;
    yuzuha_transform.position = Vector2(0.f, 0.f);
    yuzuha_transform.scale = Vector2(0.2f, 0.2f);

    Sprite sprite;
    sprite.texture_path = RESOURCE_PATH"/yuzuha.jpg";
    sprite.flipped_x = true;

    registry.emplace<Transform>(yuzuha, yuzuha_transform);
    registry.emplace<Sprite>(yuzuha, sprite);

    player_entity = yuzuha;

    shape_renderer = registry.ctx().get<ShapeRenderer *>();
    main_camera = registry.ctx().get<OrthographicCamera *>();
}

void MyScene::update(float delta) {
    if (direction.length() > 0) {
        direction.normalize();
        auto &transform = registry.get<Transform>(player_entity);
        transform.position.x += direction.x * player_speed * delta;
        transform.position.y += direction.y * player_speed * delta;


        if (transform.position != position_cache) {
            main_camera->set_position(transform.position);
        }
        position_cache = transform.position;
    }

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
        rect.x = 200 +  120;
        rect.y = 100;
        rect.width = 80;
        rect.height = 60;

        Color c;
        c.r = 1.0f;
        c.g =  0.2f;
        c.b = 0.2f;
        c.a = 1.0f;

        shape_renderer->draw_rect(rect, c);


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
