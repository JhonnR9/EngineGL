//
// Created by jhone on 22/03/2026.
//

#ifndef MY_SCENE_H
#define MY_SCENE_H
#include <memory>

#include "scene.h"
#include "graphics/sprite_batch.h"
#include "graphics/texture_2d.h"


class MyScene final : public Scene{

    std::unique_ptr<SpriteBatch> batch{nullptr};
    std::shared_ptr<Texture2D> texture1;
    std::shared_ptr<Texture2D> texture2;

    const int num_draws{2000};
    unsigned int seed{0};

public:
    MyScene(App &app);
    void init() override;

    void update(float delta) override;

    void render() override;

    void resize(int width, int height) override;
};



#endif //MY_SCENE_H
