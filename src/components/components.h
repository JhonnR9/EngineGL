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
    Color color = {255, 255, 255};
    bool flipped = false;
    Vector2 origin = Vector2(0, 0);
};

#endif //COMPONENTS_H
