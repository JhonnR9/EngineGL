//
// Created by jhone on 21/03/2026.
//

#ifndef RENDERER_H
#define RENDERER_H
#include "Texture.h"
#include "vector_2.h"

class Renderer {

public:
    Renderer();

    void draw_sprite(Texture* texture, Vector2 position, Vector2 scale, Vector2 rotation, Vector2 origin);
};


#endif //RENDERER_H
