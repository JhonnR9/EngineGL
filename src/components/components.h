#ifndef COMPONENTS_H
#define COMPONENTS_H
#include <string>

#include "utils/vector2.h"

struct Color {
    float r, g, b, a;
};

struct Rect {
    float x{0}, y{0}, width{0}, height{0};
};

struct Transform {
    Vector2 position = Vector2(0, 0);
    Vector2 scale = Vector2(1, 1);
    float rotation = 0.0f;
};

struct Sprite {
    std::string texture_path;
    Color color = {1.f, 1.f, 1.f, 1.0f};
    bool flipped_x = false;
    bool flipped_y = false;
    Rect src_rect = Rect();
};

#endif //COMPONENTS_H
