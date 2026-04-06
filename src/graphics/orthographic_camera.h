//
// Created by jhone on 24/03/2026.
//

#ifndef ORTHOGRAPHIC_CAMERA_H
#define ORTHOGRAPHIC_CAMERA_H


#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "platforms/window.h"
#include "utils/vector2.h"

enum class OriginMode {
    CENTER,
    TOP_LEFT,
};
class OrthographicCamera {


    glm::vec2 position = {0.0f, 0.0f};
    float rotation = 0.0f;
    float zoom = 1.0f;
    bool dirty{false};
    OriginMode origin{OriginMode::CENTER};

    Size view_size{800, 600};

    glm::mat4 projection = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);

public:
    OrthographicCamera(Size view_size);

    Rect get_view_rect();

    void set_view_rect(Rect view_rect);

    void update_projection();

    bool is_dirty() {
        return dirty;
    }

    void set_origin(const OriginMode origin) {
        this->origin = origin;
        dirty = true;
    }

    void set_position(Vector2 pos);

    Vector2 get_position() {
        return Vector2(position.x, position.y);
    }

    void set_zoom(const float z) {
        zoom = z;
        dirty = true;
    }

    float get_zoom() {
        return zoom;
    }

    void set_rotation(const float r) {
        rotation = r;
        dirty = true;
    }

    Size get_view_size() {
        return view_size;
    }

    void follow(Vector2 target, Vector2 deadzone, float delta, float smooth = 5.0f);

    void apply_zoom(bool zoom_in, bool zoom_out, float delta, float zoom_speed = 1.0f);

    void update_view();

    glm::mat4 get_view_projection();
};


#endif //ORTHOGRAPHIC_CAMERA_H
