//
// Created by jhone on 21/03/2026.
//

#ifndef SPRITE_H
#define SPRITE_H
#include "vector2.h"

class Texture2D;

class Sprite {
    Vector2 position;
    Vector2 scale;
    float rotation;
    Vector2 size;
    Texture2D *texture;
    bool flipped_x{false}, flipped_y{false};
    Vector2 origin;
    Vector2 texture_region;

public:
    Vector2 get_position() const {
        return position;
    }

    void set_position(const Vector2 &position) {
        this->position = position;
    }

    Vector2 get_scale() const {
        return scale;
    }

    void set_scale(const Vector2 &scale) {
        this->scale = scale;
    }

    float get_rotation() const {
        return rotation;
    }

    void set_rotation(const float rotation) {
        this->rotation = rotation;
    }

    Vector2 get_size() const {
        return size;
    }

    void set_size(const Vector2 &size) {
        this->size = size;
    }

    Texture2D *get_texture() const {
        return texture;
    }

    void set_texture(Texture2D *const texture);

    Vector2 get_origin() const {
        return origin;
    }

    void set_origin(const Vector2 &origin) {
        this->origin = origin;
    }

    [[nodiscard]] bool is_flipped_x() const {
        return flipped_x;
    }

    void set_flipped_x(const bool flipped_x) {
        this->flipped_x = flipped_x;
    }

    [[nodiscard]] bool is_flipped_y() const {
        return flipped_y;
    }

    void set_flipped_y(bool flipped_y) {
        this->flipped_y = flipped_y;
    }

    Vector2 get_texture_region() const {
        return texture_region;
    }

    void set_texture_region(const Vector2 &texture_region) {
        this->texture_region = texture_region;
    }

    Sprite();
};


#endif //SPRITE_H
