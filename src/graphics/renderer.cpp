#include "graphics/renderer.h"

#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include  "vector2.h"

Renderer::Renderer(float screenWidth, float screenHeight) {
    pipeline.projection = glm::ortho(0.0f, screenWidth, screenHeight, 0.0f, -1.0f, 1.0f);

    pipeline.material = std::make_unique<Material>();

    if (!create_default_shader()) {
        std::cerr << "Error creating default shader" << std::endl;
    }

    setup_buffers();
}

void Renderer::setup_buffers() {
    if (!create_vertex_array_object() || !create_vertex_buffer() || !create_index_buffer() || !
        create_color_vertex_instance_buffer() || !create_mpv_vertex_instance_buffer()
    ) {
        std::cerr << "Failed to create buffers" << std::endl;
    }
    glBindVertexArray(0);

}

bool Renderer::create_vertex_array_object() {
    glGenVertexArrays(1, &pipeline.vao);
    glBindVertexArray(pipeline.vao);
    return pipeline.vao != 0;
}

bool Renderer::create_vertex_buffer() {
    constexpr float vertices[] = {
        -0.5f, -0.5f, 0.0f, 0.0f,
        0.5f, -0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, 1.0f, 1.0f,
        -0.5f, 0.5f, 0.0f, 1.0f
    };

    glGenBuffers(1, &pipeline.vbo);
    glBindBuffer(GL_ARRAY_BUFFER, pipeline.vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, static_cast<void *>(nullptr));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, reinterpret_cast<void *>(sizeof(float) * 2));

    return pipeline.vbo != 0;
}

bool Renderer::create_color_vertex_instance_buffer() {
    glGenBuffers(1, &pipeline.color_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, pipeline.color_vbo);
    glBufferData(GL_ARRAY_BUFFER, pipeline.MAX_INSTANCES * sizeof(glm::vec4), nullptr, GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), static_cast<void *>(nullptr));
    glVertexAttribDivisor(2, 1);

    return pipeline.color_vbo != 0;
}

bool Renderer::create_mpv_vertex_instance_buffer() {
    glGenBuffers(1, &pipeline.mvp_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, pipeline.mvp_vbo);
    glBufferData(GL_ARRAY_BUFFER, pipeline.MAX_INSTANCES * sizeof(glm::mat4), nullptr, GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), static_cast<void *>(0));
    glVertexAttribDivisor(3, 1);

    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), reinterpret_cast<void *>(sizeof(glm::vec4)));
    glVertexAttribDivisor(4, 1);

    glEnableVertexAttribArray(5);
    glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), reinterpret_cast<void *>(sizeof(glm::vec4) * 2));
    glVertexAttribDivisor(5, 1);

    glEnableVertexAttribArray(6);
    glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), reinterpret_cast<void *>(sizeof(glm::vec4) * 3));
    glVertexAttribDivisor(6, 1);

    return pipeline.mvp_vbo != 0;
}

bool Renderer::create_index_buffer() {
    const int indices[] = {
        0, 1, 2,
        2, 3, 0
    };

    glGenBuffers(1, &pipeline.ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pipeline.ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    return pipeline.ebo != 0;
}

bool Renderer::create_default_shader() const {
    auto shader = std::make_unique<Shader>(SourcePath(RESOURCE_PATH"/default_shader.vert", RESOURCE_PATH"/default_shader.frag"));

    if (!shader->get_program()) {
        std::cout << "Failed to create shader" << std::endl;
        pipeline.material->set_shader(nullptr);
        return false;
    }

    pipeline.material->set_shader(std::move(shader));
    return true;
}

void Renderer::begin() {
    glBindVertexArray(pipeline.vao);
    pipeline.instances.clear();

}

void Renderer::draw_texture(Texture2D *texture, Vector2 position, Vector2 scale,
                           float rotation, Vector2 origin, Color color) {

    if (pipeline.instances.size() >= pipeline.MAX_INSTANCES) {
        flush();
    }

    float width  = (float)texture->get_width();
    float height = (float)texture->get_height();

    float pivotX = (origin.x / width)  - 0.5f;
    float pivotY = (origin.y / height) - 0.5f;

    glm::mat4 model = glm::mat4(1.0f);

    model = glm::translate(model, glm::vec3(position.x, position.y, 0.0f));

    model = glm::translate(model, glm::vec3(pivotX * width * scale.x,
                                            pivotY * height * scale.y,
                                            0.0f));

    model = glm::rotate(model, glm::radians(rotation), glm::vec3(0, 0, 1));
    model = glm::scale(model, glm::vec3(scale.x * width,
                                        scale.y * height,
                                        1.0f));

    model = glm::translate(model, glm::vec3(-pivotX, -pivotY, 0.0f));

    InstanceData instance;
    instance.mvp = pipeline.projection * model;

    instance.color[0] = color.r;
    instance.color[1] = color.g;
    instance.color[2] = color.b;
    instance.color[3] = color.a;

    if (pipeline.material->get_texture() != texture && pipeline.material->get_texture() != nullptr) {
        flush();
    }

    pipeline.material->set_texture(texture);
    pipeline.instances.push_back(instance);
}


void Renderer::flush() {
    end();
    pipeline.instances.clear();

}

void Renderer::end() {
    if (pipeline.instances.empty()) {
        return;
    }

    std::vector<glm::mat4> mvp_data;
    std::vector<glm::vec4> color_data;
    mvp_data.reserve(pipeline.instances.size());
    color_data.reserve(pipeline.instances.size());

    for (const auto&[mvp, color] : pipeline.instances) {
        mvp_data.push_back(mvp);
        color_data.push_back(color);
    }

    glBindBuffer(GL_ARRAY_BUFFER, pipeline.mvp_vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0,
        pipeline.instances.size() * sizeof(glm::mat4),
        mvp_data.data()
    );

    glBindBuffer(GL_ARRAY_BUFFER, pipeline.color_vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0,
        pipeline.instances.size() * sizeof(glm::vec4),
        color_data.data()
    );

    pipeline.material->use();

    glDrawElementsInstanced(
        GL_TRIANGLES,
        6,
        GL_UNSIGNED_INT,
        nullptr,
        pipeline.instances.size()
    );

}

Renderer::~Renderer() {
    glDeleteBuffers(1, &pipeline.vbo);
    glDeleteBuffers(1, &pipeline.ebo);
    glDeleteBuffers(1, &pipeline.color_vbo);
    glDeleteBuffers(1, &pipeline.mvp_vbo);
    glDeleteVertexArrays(1, &pipeline.vao);
}
