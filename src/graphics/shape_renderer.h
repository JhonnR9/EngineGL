//
// Created by jhone on 23/03/2026.
//

#ifndef SHAPE_RENDERER_H
#define SHAPE_RENDERER_H
#include <memory>
#include <vector>

#include "orthographic_camera.h"
#include "shader.h"
#include "components/components.h"
#include "glm/vec2.hpp"
#include "glm/vec4.hpp"


class ShapeRenderer {
    struct QuadInstance {
        glm::vec2 position;
        glm::vec2 origin;
        float rotation;
        glm::vec2 scale;
        glm::vec4 color;
        int shape_type;
    };

    struct Pipeline {
        GLuint vao{0}, quad_vbo{0}, quad_ebo{0}, quad_ibo{0};
        unsigned int MAX_INSTANCES{4096};
        std::unique_ptr<Shader> shader;
        std::vector<QuadInstance> quad_instances;
        glm::mat4 projection{1.0f};
        OrthographicCamera* camera{nullptr};
    };

    enum ShapeType {
        Rectangle,
        Eclipse
    };

    std::unique_ptr<Pipeline> pipeline{nullptr};

public:
    ShapeRenderer(OrthographicCamera* camera);

private:
    bool create_vertex_array_object();
    bool create_quad_static_buffers();
    bool create_quad_dynamic_buffers();

    void setup_buffers();
    void draw_shape(Rect rect, Color color, Vector2 origin={0.0, 0.0}, float rotation=0.0f, ShapeType type= Rectangle);

public:
    void flush();
    void begin();
    void end();
    void draw_rect(Rect rect, Color color, Vector2 origin={0.0, 0.0}, float rotation=0.0f);
    void draw_eclipse(Ellipse ellipse, Color color);
};


#endif //SHAPE_RENDERER_H
