#ifndef RENDERER_H
#define RENDERER_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>
#include <memory>
#include "graphics/texture_2d.h"
#include "graphics/material.h"

struct Color {
    float r, g, b, a;
};



struct InstanceData {
    glm::mat4 mvp;
    glm::vec4 color;
};
class Vector2;
class Renderer {
    struct Pipeline {
        GLuint vao{0};
        GLuint vbo{0};
        GLuint ebo{0};
        GLuint color_vbo{0};
        GLuint mvp_vbo{0};
        glm::mat4 projection{1.0f};
        std::unique_ptr<Material> material{nullptr};
        unsigned int MAX_INSTANCES{1024};
        std::vector<InstanceData> instances;
        Texture2D* current_texture{nullptr};
    };


    Pipeline pipeline;
public:
    Renderer(float screenWidth, float screenHeight);

    ~Renderer();

    void begin();

    void draw_texture(Texture2D *texture, Vector2 position, Vector2 scale,
                      float rotation, Vector2 origin, Color color);
    void flush();
    void end();

private:
    void setup_buffers();

    bool create_vertex_array_object();

    bool create_vertex_buffer();

    bool create_color_vertex_instance_buffer();
    bool create_mpv_vertex_instance_buffer();

    bool create_index_buffer();

    bool create_default_shader() const;


    std::vector<InstanceData> drawCalls;
};

#endif
