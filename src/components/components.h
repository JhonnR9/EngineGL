#ifndef COMPONENTS_H
#define COMPONENTS_H
#include <string>
#include <unordered_set>
#include "utils/vector2.h"
#include "entt/entt.hpp"

struct Color {
    float r{1.0f}, g{1.0f}, b{1.0f}, a{1.0f};
};

struct Rect {
    float x{0}, y{0}, width{0.f}, height{0.f};
};

struct Ellipse {
    float cx{0}, cy{0}, rx{0}, ry{0};
};

struct Line {
    Vector2 start;
    Vector2 end;
    float thickness;
};

struct ZIndex {
    float value = 0.0f;
};

struct Transform {
    Vector2 position = Vector2(0, 0);
    Vector2 scale = Vector2(1.0f, 1.0f);
    float rotation = 0.0f;
};

struct Sprite {
    std::string texture_path;
    Color color = {1.f, 1.f, 1.f, 1.0f};
    bool flipped_x = false;
    bool flipped_y = false;
    Rect src_rect = Rect();
};

struct BoxCollider2D {
    float width = 0;
    float height = 0;
    bool is_colliding = false;
    bool is_trigger = false;
    bool is_static = false;
    bool sync_size_with_sprite = true;
    std::unordered_set<entt::entity> colliding_entities;

    BoxCollider2D() = default;
    BoxCollider2D(float width, float height, bool is_colliding, bool is_trigger, bool is_static, bool sync_size_with_sprite)
        : width(width), height(height), is_colliding(is_colliding),
          is_trigger(is_trigger), is_static(is_static), sync_size_with_sprite(sync_size_with_sprite) {}
};

#endif //COMPONENTS_H
