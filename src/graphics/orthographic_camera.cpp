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


glm::mat4 OrthographicCamera::get_view_projection() {
    if (dirty) {
        update_view();
        update_projection();
        dirty = false;
    }
    return projection * view;
}
