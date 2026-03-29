#ifndef RENDERER_H
#define RENDERER_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>
#include <memory>

#include "font.h"
#include "orthographic_camera.h"
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
        float z_index{0.0f};
        int shape_type{-1};
    };

    struct Pipeline {
        GLuint vao{0};
        GLuint vbo{0};
        GLuint ebo{0};
        GLuint instance_vbo{0};
        std::unique_ptr<Shader> shader{nullptr};
        unsigned int MAX_INSTANCES{4096};
        std::vector<InstanceData> instances;
        static constexpr int MAX_TEXTURE_SLOTS = 16;
        std::vector<Texture2D *> texture_slots;
        OrthographicCamera *camera{nullptr};
    };

    enum class ShapeType {
        Rectangle,
        Ellipse,
        Line
    };

    Pipeline pipeline;

public:
    SpriteBatch(OrthographicCamera *camera);

    ~SpriteBatch();

    void begin();

    void draw_texture(
        Texture2D *texture,
        Vector2 position,
        Vector2 scale,
        float rotation,
        Vector2 origin,
        Color color,
        Rect sourceRect = Rect(),
        bool flip_x = false,
        bool flip_y = false,
        float z_index = 0.0f
    );

    void draw_shape(Rect rect, Color color, Vector2 origin = {0.0, 0.0}, float rotation = 0.0f,
                    ShapeType type = ShapeType::Rectangle, float z_index = 0.0f);

    void draw_rect(Rect rect, Color color, Vector2 origin = {0.0, 0.0}, float rotation = 0.0f, float z_index = 0.0f);

    void draw_ellipse(Ellipse ellipse, Color color, float z_index = 0.0f);

    void draw_line(Vector2 start, Vector2 end, float thickness, Color color, float z_index = 0.0f);

    void draw_texture(Texture2D *texture, Vector2 position, float z_index = 0.0f);

    void draw_texture(Texture2D *texture, Vector2 position, Vector2 scale, float z_index = 0.0f);

    void draw_texture(Texture2D *texture, Vector2 position, Vector2 scale, float rotation, float z_index = 0.0f);

    void draw_texture(Texture2D *texture, Vector2 position, Vector2 scale, float rotation, Color color,
                      float z_index = 0.0f);

    void draw_text(Font &font,
                   const std::string &text,
                   Vector2 position,
                   float scale,
                   Color color,
                   float z_index);

    void flush();

    void end();

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
