//
// Created by jhone on 21/03/2026.
//

#include "renderer.h"
#include "texture_2d.h"
#include "vector2.h"
#include "material.h"
#include "sprite.h"

Renderer::Renderer(): currentTexture(nullptr) {
    Vertex vertices[4] = {
        {{-0.5f, -0.5f}, {0.0f, 0.0f}},
        {{0.5f, -0.5f}, {1.0f, 0.0f}},
        {{0.5f, 0.5f}, {1.0f, 1.0f}},
        {{-0.5f, 0.5f}, {0.0f, 1.0f}}
    };

    static constexpr GLuint indices[6] = {
        0, 1, 2,
        2, 3, 0
    };
}

void Renderer::begin() {
}

void Renderer::end() {
}

void Renderer::flush() {
}

void Renderer::draw_texture(Texture2D *texture, Vector2 &position, Vector2 &scale, float rotation, Vector2 &origin,
                            Vector2 region) {
    //TODO draw textures;
}

void Renderer::draw_sprite(Sprite *sprite) {
    if (sprites.size() < MAX_SPRITES) {

    }
}
