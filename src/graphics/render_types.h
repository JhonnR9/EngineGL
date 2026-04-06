//
// Created by jhone on 05/04/2026.
//

#ifndef RENDER_TYPES_H
#define RENDER_TYPES_H

#include <glm/glm.hpp>
#include "utils/vector2.h"

struct InstanceData {
    glm::vec2 position;
    glm::vec2 origin;
    float rotation;
    glm::vec2 scale;
    glm::vec4 color;
    glm::vec4 region;
    float tex_index;
    int flip{0};
    float z_index{0.0f};
    int shape_type{-1};
};

struct Color { float r{1.0f}, g{1.0f}, b{1.0f}, a{1.0f}; };
struct Rect { float x{0}, y{0}, width{0.f}, height{0.f}; };
struct Ellipse { float cx{0}, cy{0}, rx{0}, ry{0}; };

struct Line {
    Vector2 start;
    Vector2 end;
    float thickness;
};

#endif //RENDER_TYPES_H
