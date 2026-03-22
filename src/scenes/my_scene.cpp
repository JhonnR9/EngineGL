//
// Created by jhone on 22/03/2026.
//

#include "my_scene.h"

#include <ctime>
#include "../utils/vector2.h"
#include "../main/app.h"
#include "glm/ext/matrix_clip_space.hpp"

MyScene::MyScene(App &app): Scene(app) {
}

void MyScene::init() {
    batch = std::make_unique<SpriteBatch>(app.get_windowed_width(), app.get_windowed_height());
    auto a = Vector2(1.f, 1.f);
    texture1 = std::make_unique<Texture2D>(RESOURCE_PATH"/yuzuha.jpg", a);

    seed = static_cast<unsigned int>(time(nullptr));
    auto projection = glm::ortho(0.0f, app.VIRTUAL_WIDTH, app.VIRTUAL_HEIGHT, 0.0f, -1.0f, 1.0f);
    batch->set_projection(projection);
}

void MyScene::update(float delta) {
}

void MyScene::render() {
    float currentTime = glfwGetTime();
    float rotation = currentTime * (0.5f + static_cast<float>(rand()) / RAND_MAX) * 20;

    srand(seed);

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    batch->begin();

    Color color;
    color.r = 1.0f;
    color.g = 1.0f;
    color.b = 1.0f;
    color.a = 1.0f;

    batch->draw_texture(
        texture1.get(),
        Vector2(1366/2, 768/2),
        Vector2(1.f, 1.f),
        rotation,
        Vector2(texture1->get_width() / 2.f, texture1->get_height() / 2.f),
        color,
        Rect());

    batch->end();
}

void MyScene::resize(int width, int height) {
    const auto projection = glm::ortho(0.0f, app.VIRTUAL_WIDTH, app.VIRTUAL_HEIGHT, 0.0f, -1.0f, 1.0f);
    batch->set_projection(projection);
}
