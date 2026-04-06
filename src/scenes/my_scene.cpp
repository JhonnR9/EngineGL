//
// Created by jhone on 22/03/2026.
//

#include "my_scene.h"
#include <ctime>
#include <filesystem>

#include "utils/vector2.h"
#include "main/app.h"
#include "input/keyboard.h"
#include "utils/tmx_reader.h"
#include "graphics/orthographic_camera.h"


void MyScene::load_assets(AssetsManager *manager) {
    std::string path = RESOURCE_PATH"/player.png";
    manager->queue_file_load(path.c_str());
}

MyScene::MyScene(App &app, entt::registry &registry): Scene(app, registry), player_entity() {
}

void MyScene::init() {
    if (registry.ctx().contains<OrthographicCamera *>()) {
        main_camera = registry.ctx().get<OrthographicCamera *>();
    }

    auto tile_entity = registry.create();
    Transform transform;
    transform.position = Vector2(300, 200);

    TileMapLayer map_layer;
    map_layer.map_path = (RESOURCE_PATH"/Tilemap/map.tmx");
    map_layer.layer_index = (0);

    registry.emplace<Transform>(tile_entity, transform);
    registry.emplace<TileMapLayer>(tile_entity, map_layer);

    auto player = registry.create();
    player_entity = player;

    Transform player_transform{};
    player_transform.position = Vector2(0.f, 0.f);

    BoxCollider2D player_collider{};
    player_collider.width = 100.f;
    player_collider.height = 100.f;


    Sprite player_sprite{};
    player_sprite.texture_path = RESOURCE_PATH"/player.png";
    player_sprite.filter = TextureFilter::Nearest;

    registry.emplace<Transform>(player, player_transform);
    registry.emplace<Color>(player, Color{1, 0, 0, 1});
    registry.emplace<BoxCollider2D>(player, player_collider);
    registry.emplace<Sprite>(player, player_sprite);
    registry.emplace<ZIndex>(player, ZIndex{0.0f});

    auto ui_text = registry.create();
    text_entity = ui_text;
    Transform label_transform;
    label_transform.position = Vector2(-0.f, -0.f);
    font = std::make_shared<Font>(RESOURCE_PATH"/Coolvetica Rg.otf", 32);

    Label label{};
    label.font = font;
    label.text = "Texto pro 123";
    label.font_size = 32;
    label.color = Color{1, 1, 1, 1};

    registry.emplace<Label>(ui_text, label);
    registry.emplace<Transform>(ui_text, label_transform);
    registry.emplace<ZIndex>(ui_text, ZIndex{1.f});
}

void MyScene::update(float delta) {
    if (delta > 0.05f) delta = 0.05f;

    auto &transform = registry.get<Transform>(player_entity);

    if (direction.length() > 0) {
        direction.normalize();
        transform.position.x += direction.x * player_speed * delta;
        transform.position.y += direction.y * player_speed * delta;
    }

    auto &label = registry.get<Label>(text_entity);
    //label.text = app.get_current_fps();

    if (main_camera) {
        Size size = main_camera->get_view_size();
        float w = static_cast<float>(size.width);
        float h = static_cast<float>(size.height);
        main_camera->follow(transform.position, Vector2(w * 0.2f, h * 0.2f), delta);

        const float current = main_camera->get_zoom();
        const float target = zoom;
        constexpr float smooth = 10.0f;

        const float new_zoom = current + (target - current) * smooth * delta;
        main_camera->set_zoom(new_zoom);
    }
}


void MyScene::on_event(const Event &e) {
    switch (e.get_type()) {
        case EventType::Key: {
            auto &ev = static_cast<const KeyEvent &>(e);
            const bool pressed = ev.action == KeyAction::Press;

            if (ev.key == Key::W) up = pressed;
            if (ev.key == Key::S) down = pressed;
            if (ev.key == Key::A) left = pressed;
            if (ev.key == Key::D) right = pressed;

            direction.x = (right ? 1.f : 0.f) - (left ? 1.f : 0.f);
            direction.y = (down ? 1.f : 0.f) - (up ? 1.f : 0.f);
            break;
        }
        case EventType::MouseWheel: {
            auto &ev = static_cast<const MouseWheelEvent &>(e);
            zoom += ev.delta * zoom_speed;
            zoom = glm::clamp<float>(zoom, 0.2f, 5.0f);
            zoom = glm::clamp<float>(zoom, 0.2, 5.0f);

            break;
        }
        default: break;
    }
}
