#ifndef RENDERER_H
#define RENDERER_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>
#include <memory>

#include "shader.h"
#include "graphics/texture_2d.h"
#include "components/components.h"

class Vector2;

class SpriteBatch {
    struct InstanceData {
        glm::vec2 position;
        glm::vec2 origin;
        float rotation;
        glm::vec2 scale;
        glm::vec4 color;
        glm::vec4 region;
        float tex_index;
        int flip{0};
    };

    struct Pipeline {
        GLuint vao{0};
        GLuint vbo{0};
        GLuint ebo{0};
        GLuint instance_vbo{0};
        glm::mat4 projection{1.0f};
        std::unique_ptr<Shader> shader{nullptr};
        unsigned int MAX_INSTANCES{4096};
        std::vector<InstanceData> instances;
        static constexpr int MAX_TEXTURE_SLOTS = 16;
        std::vector<Texture2D *> texture_slots;
    };


    Pipeline pipeline;

public:
    SpriteBatch(float screenWidth, float screenHeight);

    ~SpriteBatch();

    void begin();

    void draw_texture(
        Texture2D *texture,
        Vector2 position,
        Vector2 scale,
        float rotation,
        Vector2 origin,
        Color color,
        Rect sourceRect=Rect(),
        bool flip_x = false,
        bool flip_y = false
    );

    void draw_texture(Texture2D *texture, Vector2 position);

    void draw_texture(Texture2D *texture, Vector2 position, Vector2 scale);

    void draw_texture(Texture2D *texture, Vector2 position, Vector2 scale, float rotation);

    void draw_texture(Texture2D *texture, Vector2 position, Vector2 scale, float rotation, Color color);

    void flush() const;

    void end() const;

    void set_projection(const glm::mat<4, 4, float> &projection);

private:
    void setup_buffers();

    bool create_vertex_array_object();

    bool create_vertex_buffer();

    bool create_instance_buffer();

    bool create_index_buffer();

    bool create_default_shader();


    std::vector<InstanceData> drawCalls;
};

#endif
