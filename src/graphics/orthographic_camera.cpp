//
// Created by jhone on 24/03/2026.
//

#include "orthographic_camera.h"

OrthographicCamera::OrthographicCamera(const float width, const float height) {
    virtual_width = width;
    virtual_height = height;

    update_projection();
    update_view();
}

void OrthographicCamera::update_projection() {
    projection = glm::ortho(
        0.0f,
        virtual_width,
        virtual_height,
        0.0f,
        -100.0f, // near
        100.0f // far
    );
}

void OrthographicCamera::set_position(Vector2 pos) {
    position = glm::vec2(pos.x, pos.y);
    dirty = true;
}

void OrthographicCamera::update_view() {
    view = glm::mat4(1.0f);

    view = glm::translate(view, glm::vec3(virtual_width / 2.0f, virtual_height / 2.0f, 0.0f));

    view = glm::scale(view, glm::vec3(zoom, zoom, 1.0f));
    view = glm::rotate(view, glm::radians(-rotation), glm::vec3(0, 0, 1));

    view = glm::translate(view, glm::vec3(-position.x, -position.y, 0.0f));
}
void OrthographicCamera::follow(Vector2 target, Vector2 deadzone, float delta, float smooth) {
    Vector2 camTarget = get_position();

    float left   = position.x - deadzone.x / 2.0f;
    float right  = position.x + deadzone.x / 2.0f;
    float top    = position.y - deadzone.y / 2.0f;
    float bottom = position.y + deadzone.y / 2.0f;

    bool outside = false;

    // X
    if (target.x < left) {
        camTarget.x = target.x + deadzone.x / 2.0f;
        outside = true;
    } else if (target.x > right) {
        camTarget.x = target.x - deadzone.x / 2.0f;
        outside = true;
    }

    // Y
    if (target.y < top) {
        camTarget.y = target.y + deadzone.y / 2.0f;
        outside = true;
    } else if (target.y > bottom) {
        camTarget.y = target.y - deadzone.y / 2.0f;
        outside = true;
    }

    if (outside) {
        position.x += (camTarget.x - position.x) * smooth * delta;
        position.y += (camTarget.y - position.y) * smooth * delta;
        dirty = true;
    }
}

void OrthographicCamera::apply_zoom(bool zoom_in, bool zoom_out, float delta, float zoom_speed) {
    if (zoom_in) zoom -= zoom_speed * delta;
    if (zoom_out) zoom += zoom_speed * delta;

    if (zoom < 0.2f) zoom = 0.2f;
    if (zoom > 5.0f) zoom = 5.0f;

    dirty = true;
}


glm::mat4 OrthographicCamera::get_view_projection() {
    if (dirty) {
        update_view();
        update_projection();
        dirty = false;
    }
    return projection * view;
}
