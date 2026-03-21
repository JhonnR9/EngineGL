//
// Created by jhone on 21/03/2026.
//

#include "sprite.h"

#include "graphics/texture_2d.h"

void Sprite::set_texture(Texture2D *const texture) {
    this->texture = texture;

    if (texture) {
        size.x = texture->get_width();
        size.y = texture->get_height();
        return;
    }

    size.x = size.y = 0;
}

Sprite::Sprite()
    : position{0.0f, 0.0f},
      scale{1.0f, 1.0f},
      rotation{0.0f},
      size{0.0f, 0.0f},
      texture{nullptr},
      origin{0.0f, 0.0f},
      texture_region{0.0f, 0.0f} {
}
