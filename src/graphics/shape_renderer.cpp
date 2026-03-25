//
// Created by jhone on 23/03/2026.
//

#include "shape_renderer.h"

#include <iostream>

ShapeRenderer::ShapeRenderer(OrthographicCamera* camera) {
    setup_buffers();
    if (!camera) {
        std::cerr << "camera pointer is null" << std::endl;
    }else {
        if (pipeline->shader) {
            pipeline->shader->set_mat4("uViewProjection", camera->get_view_projection());
        }
    }
    this->pipeline->camera =camera;
}


bool ShapeRenderer::create_vertex_array_object() {
    glGenVertexArrays(1, &pipeline->vao);
    glBindVertexArray(pipeline->vao);
    return pipeline->vao != 0;
}

bool ShapeRenderer::create_quad_static_buffers() {
    glBindVertexArray(pipeline->vao);

    constexpr float vertices[] = {
        -0.5f, -0.5f,
        0.5f, -0.5f,
        0.5f, 0.5f,
        -0.5f, 0.5f,
    };

    glGenBuffers(1, &pipeline->quad_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, pipeline->quad_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *) 0);

    const int indices[] = {
        0, 1, 2,
        2, 3, 0
    };

    glGenBuffers(1, &pipeline->quad_ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pipeline->quad_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    return pipeline->quad_vbo != 0 && pipeline->quad_ebo != 0;
}

bool ShapeRenderer::create_quad_dynamic_buffers() {
    glBindVertexArray(pipeline->vao);
    glGenBuffers(1, &pipeline->quad_ibo);
    glBindBuffer(GL_ARRAY_BUFFER, pipeline->quad_ibo);

    glBufferData(GL_ARRAY_BUFFER, pipeline->MAX_INSTANCES * sizeof(QuadInstance), nullptr, GL_DYNAMIC_DRAW);
    // location = 1  Position
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(QuadInstance), (void *) offsetof(QuadInstance, position));
    glVertexAttribDivisor(1, 1);

    // location = 2 Origin
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(QuadInstance), (void *) offsetof(QuadInstance, origin));
    glVertexAttribDivisor(2, 1);

    // location = 3 Rotation
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(QuadInstance), (void *) offsetof(QuadInstance, rotation));
    glVertexAttribDivisor(3, 1);

    // location = 4 Scale
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, sizeof(QuadInstance), (void *) offsetof(QuadInstance, scale));
    glVertexAttribDivisor(4, 1);

    // location = 5 Color
    glEnableVertexAttribArray(5);
    glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(QuadInstance), (void *) offsetof(QuadInstance, color));
    glVertexAttribDivisor(5, 1);

    // location = 6 ShapeType
    glEnableVertexAttribArray(6);
    glVertexAttribIPointer(6, 1,GL_INT, sizeof(QuadInstance), (void *) offsetof(QuadInstance, shape_type));
    glVertexAttribDivisor(6, 1);

    return pipeline->quad_ibo != 0;
}

void ShapeRenderer::setup_buffers() {
    pipeline = std::make_unique<Pipeline>();
    pipeline->shader = std::make_unique<Shader>(
       SourcePath(RESOURCE_PATH"/shape_renderer.vert", RESOURCE_PATH"/shape_renderer.frag"));

    if (!create_vertex_array_object()) {
        std::cerr << "Error creating vertex array object" << std::endl;
    }

    if (!create_quad_static_buffers() || !create_quad_dynamic_buffers()) {
        std::cerr << "Error creating quad buffers" << std::endl;
    }


}

void ShapeRenderer::draw_shape(Rect rect, Color color, Vector2 origin, float rotation, ShapeType type) {
    if (rect.width <= 0.0f || rect.height <= 0.0f) {
        return;
    }

    if (pipeline->quad_instances.size() >= pipeline->MAX_INSTANCES) {
        flush();
    }

    QuadInstance instance;

    instance.position.x = rect.x;
    instance.position.y = rect.y;

    instance.origin.x = origin.x;
    instance.origin.y = origin.y;

    instance.rotation = rotation;

    instance.scale.x = rect.width;
    instance.scale.y = rect.height;

    instance.color.r = color.r;
    instance.color.g = color.g;
    instance.color.b = color.b;
    instance.color.a = color.a;

    instance.shape_type = type;

    pipeline->quad_instances.push_back(instance);
}

void ShapeRenderer::flush() {
    end();
    begin();
}

void ShapeRenderer::begin() {
    glBindVertexArray(pipeline->vao);
    pipeline->quad_instances.clear();

    if (pipeline->camera ) {
        pipeline->shader->use();
        pipeline->shader->set_mat4("uViewProjection", pipeline->camera->get_view_projection());
    }

}

void ShapeRenderer::end() {
    pipeline->shader->use();
    if (!pipeline->quad_instances.empty()) {

        glBindBuffer(GL_ARRAY_BUFFER, pipeline->quad_ibo);
        glBufferSubData(GL_ARRAY_BUFFER, 0, pipeline->quad_instances.size() * sizeof(QuadInstance),
                        pipeline->quad_instances.data());

        glDrawElementsInstanced(
            GL_TRIANGLES,
            6,
            GL_UNSIGNED_INT,
            nullptr,
            pipeline->quad_instances.size()
        );
    }
}

void ShapeRenderer::draw_rect(Rect rect, Color color, Vector2 origin, float rotation) {
    draw_shape(rect, color, origin, rotation, Rectangle);
}

void ShapeRenderer::draw_eclipse(Ellipse ellipse, Color color) {
    Rect rect;
    rect.x = ellipse.cx;
    rect.y = ellipse.cy;
    rect.width = ellipse.rx * 2.f;
    rect.height = ellipse.ry * 2.f;

    draw_shape(rect, color, Vector2(0.5f, 0.5f), 0.0f, Eclipse);
}
