//
// Created by jhone on 22/03/2026.
//

#include "my_scene.h"

#include <ctime>
#include "vector2.h"
#include "app.h"
#include "glm/ext/matrix_clip_space.hpp"

MyScene::MyScene(App &app): Scene(app) {
}

void MyScene::init() {
    batch = std::make_unique<SpriteBatch>(app.get_windowed_width(), app.get_windowed_height());
    auto a = Vector2(0.25f, 0.25f);
    texture1 = std::make_unique<Texture2D>(RESOURCE_PATH"/yuzuha.jpg" ,a);

    seed = static_cast<unsigned int>(time(nullptr));
    auto projection = glm::ortho(0.0f, app.VIRTUAL_WIDTH, app.VIRTUAL_HEIGHT, 0.0f, -1.0f, 1.0f);
    batch->set_projection(projection) ;
}

void MyScene::update(float delta){

}

void MyScene::render() {

    float currentTime = glfwGetTime();
    float rotation = currentTime * (0.5f + static_cast<float>(rand()) / RAND_MAX) * 20;

    srand(seed);

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    batch->begin();

    for (int i = 0; i < num_draws; ++i) {
        float x = static_cast<float>(rand() % app.get_windowed_width());
        float y = static_cast<float>(rand() % app.get_windowed_height());

        Color c;
        c.r = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
        c.g = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
        c.b = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
        c.a =static_cast<float>(rand()) / static_cast<float>(RAND_MAX);

        batch->draw_texture(
            texture1.get(),
            Vector2(x, y),
            Vector2(1.f, 1.f),
            rotation,
            Vector2(texture1->get_width() / 2.f, texture1->get_height() / 2.f),
            c,
            Rect()
        );

    }



    batch->end();

}

void MyScene::resize(int width, int height) {
    const auto projection = glm::ortho(0.0f, app.VIRTUAL_WIDTH, app.VIRTUAL_HEIGHT, 0.0f, -1.0f, 1.0f);
    batch->set_projection(projection);
}
