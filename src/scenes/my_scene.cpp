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
    main_camera = registry.ctx().get<OrthographicCamera *>();

    font = std::make_unique<Font>(RESOURCE_PATH"/Slumber Dog.ttf", 64);

    std::srand(static_cast<unsigned>(std::time(nullptr)));

    // --- PLAYER ---
    auto player = registry.create();
    player_entity = player;

    Transform player_transform;
    player_transform.position = Vector2(0.f, 0.f);

    BoxCollider2D player_collider;
    player_collider.width = 100.f;
    player_collider.height = 100.f;

    Rect player_rect;
    player_rect.width = 100.f;
    player_rect.height = 100.f;

    registry.emplace<Transform>(player, player_transform);
    registry.emplace<Rect>(player, player_rect);
    registry.emplace<ZIndex>(player, ZIndex{1});
    registry.emplace<Color>(player, Color{1, 0, 0, 1});
    registry.emplace<BoxCollider2D>(player, player_collider);


    // --- SPAWN MASSIVO ---
    /* const int ENTITY_COUNT = 100;
     const float WORLD_SIZE = 1000.f;

     for (int i = 0; i < ENTITY_COUNT; i++) {
         float x = ((std::rand() / (float) RAND_MAX) * 2.f - 1.f) * WORLD_SIZE;
         float y = ((std::rand() / (float) RAND_MAX) * 2.f - 1.f) * WORLD_SIZE;

         float size = 20.f + (std::rand() / (float) RAND_MAX) * 60.f;

         float r = (std::rand() / (float) RAND_MAX);
         float g = (std::rand() / (float) RAND_MAX);
         float b = (std::rand() / (float) RAND_MAX);

         spawn_box(
             registry,
             Vector2(x, y),
             size,
             size,
             Color{r, g, b, 1.0f},
             false
         );
     }*/

    // --- ALGUNS OBJETOS GRANDES (opcional) ---
    auto s =Vector2(0.02f, 0.02f);
    texture = std::make_unique<Texture2D>(RESOURCE_PATH"/hutao.png", s);
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
    batch.begin();
   // batch.draw_texture(texture.get(), position_cache);
    //batch.draw_text(*font.get(), "Texto nao pro", Vector2::one(), 1.0f, Color{1.0f, 1.f, 0.f, 1.f}, 100);
    batch.draw_line(Vector2::zero(), Vector2(400.f, 0.f), 2.f, Color{1.0f, 1.f, 0.f, 1.f}, 5.f);
    Ellipse ellipse;
    ellipse.rx = 50, ellipse.ry = 50;
    batch.draw_eclipse(ellipse, Color{1.0f, 1.f, 0.f, 1.f}, 20.f);
    batch.end();
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
