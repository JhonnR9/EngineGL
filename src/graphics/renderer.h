//
// Created by jhone on 21/03/2026.
//

#ifndef RENDERER_H
#define RENDERER_H
#include <memory>
#include <vector>

#include "glm/vec2.hpp"
#include "glm/ext/matrix_float4x4.hpp"


class Sprite;
class Material;
class Texture2D;
class Vector2;

class Renderer {
    struct Vertex {
        glm::vec2 pos;
        glm::vec2 texCoord;
    };

    struct InstanceData {
        glm::mat4 mvp;
        glm::vec3 color;
        float textureIndex;
    };

    struct Batch {
        Texture2D* texture;
        std::vector<InstanceData> instances;
    };


    std::unique_ptr<Material> material;
    std::vector<Sprite> sprites;


    Texture2D *currentTexture;
    const int MAX_SPRITES = 1000;

public:
    Renderer();

    void begin();

    void end();

    void flush();

    void draw_texture(Texture2D *texture, Vector2 &position, Vector2 &scale, float rotation, Vector2 &origin,Vector2 region);

    void draw_sprite(Sprite *sprite);
};


#endif //RENDERER_H
