//
// Created by jhone on 22/03/2026.
//

#include "my_scene.h"
#include <ctime>
#include "utils/vector2.h"
#include "main/app.h"
#include "input/keyboard.h"
#include "utils/tmx_reader.h"


MyScene::MyScene(App &app, entt::registry &registry): Scene(app, registry), player_entity() {

}

void MyScene::init() {
    if (registry.ctx().contains<OrthographicCamera*>()) {
        main_camera = registry.ctx().get<OrthographicCamera*>();
    }

    auto map = std::make_shared<TMXReader>(RESOURCE_PATH"/Tilemap/map.tmx");

    auto tile_entity = registry.create();
    Transform transform;
    transform.position = Vector2(300, 200);

    TileMapLayer tilemaplayer;
    tilemaplayer.setMap(map);

    if ( tilemaplayer.isValid()) {
        tilemaplayer.setLayerIndex(0);
    };
    registry.emplace<Transform>(tile_entity, transform);
    registry.emplace<TileMapLayer>(tile_entity, tilemaplayer);

    std::srand(static_cast<unsigned>(std::time(nullptr)));

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
    if (delta > 0.05f) delta = 0.05f;

    auto &transform = registry.get<Transform>(player_entity);

    if (direction.length() > 0) {
        direction.normalize();
        transform.position.x += direction.x * player_speed * delta;
        transform.position.y += direction.y * player_speed * delta;
    }

    main_camera->follow(transform.position, Vector2(app.VIRTUAL_WIDTH * 0.2f, app.VIRTUAL_HEIGHT * 0.2f), delta);

}



void MyScene::render(SpriteBatch &batch) {

}


void MyScene::on_key_event(int key, int scancode, int action, int mods) {
    bool pressed = action != 0;

    if (key == Key::W) up = pressed;
    if (key == Key::S) down = pressed;
    if (key == Key::A) left = pressed;
    if (key == Key::D) right = pressed;

    direction.x = (right ? 1.f : 0.f) - (left ? 1.f : 0.f);
    direction.y = (down ? 1.f : 0.f) - (up ? 1.f : 0.f);

}

void MyScene::on_mouse_button_event(int button, int action, int x, int y) {

}

void MyScene::on_mouse_move_event(int x, int y) {

}

void MyScene::on_mouse_wheel_event(int delta) {
    zoom += delta * zoom_speed * 0.001f;

    if (zoom < 0.2f) zoom = 0.2f;
    if (zoom > 5.0f) zoom = 5.0f;

    if (main_camera) {
        main_camera->set_zoom(zoom);
    }
}
