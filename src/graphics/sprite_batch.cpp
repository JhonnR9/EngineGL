#include "sprite_batch.h"

#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include  "vector2.h"

SpriteBatch::SpriteBatch(float screenWidth, float screenHeight) {
    pipeline.projection = glm::ortho(0.0f, screenWidth, screenHeight, 0.0f, -1.0f, 1.0f);

    if (!create_default_shader()) {
        std::cerr << "Error creating default shader" << std::endl;
    }

    setup_buffers();
}

void SpriteBatch::setup_buffers() {
    if (!create_vertex_array_object() || !create_vertex_buffer() || !create_index_buffer() ||
        !create_instance_buffer()
    ) {
        std::cerr << "Failed to create buffers" << std::endl;
    }
    glBindVertexArray(0);
}

bool SpriteBatch::create_vertex_array_object() {
    glGenVertexArrays(1, &pipeline.vao);
    glBindVertexArray(pipeline.vao);
    return pipeline.vao != 0;
}

bool SpriteBatch::create_vertex_buffer() {
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

bool SpriteBatch::create_instance_buffer() {
    glGenBuffers(1, &pipeline.instance_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, pipeline.instance_vbo);

    glBufferData(
        GL_ARRAY_BUFFER,
        pipeline.MAX_INSTANCES * sizeof(InstanceData),
        nullptr,
        GL_DYNAMIC_DRAW
    );

    std::size_t offset = 0;

    for (int i = 0; i < 4; i++) {
        glEnableVertexAttribArray(3 + i);
        glVertexAttribPointer(
            3 + i,
            4,
            GL_FLOAT,
            GL_FALSE,
            sizeof(InstanceData),
            reinterpret_cast<void *>(offset)
        );
        glVertexAttribDivisor(3 + i, 1);

        offset += sizeof(glm::vec4);
    }

    glEnableVertexAttribArray(7);
    glVertexAttribPointer(
        7,
        4,
        GL_FLOAT,
        GL_FALSE,
        sizeof(InstanceData),
        reinterpret_cast<void *>(offsetof(InstanceData, color))
    );
    glVertexAttribDivisor(7, 1);

    glEnableVertexAttribArray(8);
    glVertexAttribPointer(
        8,
        4,
        GL_FLOAT,
        GL_FALSE,
        sizeof(InstanceData),
        reinterpret_cast<void *>(offsetof(InstanceData, region))
    );
    glVertexAttribDivisor(8, 1);

    glEnableVertexAttribArray(9);
    glVertexAttribPointer(
        9, 1, GL_FLOAT, GL_FALSE,
        sizeof(InstanceData),
        reinterpret_cast<void *>(offsetof(InstanceData, texIndex))
    );
    glVertexAttribDivisor(9, 1);

    return pipeline.instance_vbo != 0;
}


bool SpriteBatch::create_index_buffer() {
    const int indices[] = {
        0, 1, 2,
        2, 3, 0
    };

    glGenBuffers(1, &pipeline.ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pipeline.ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    return pipeline.ebo != 0;
}

bool SpriteBatch::create_default_shader()  {
    auto shader = std::make_unique<Shader>(
        SourcePath(RESOURCE_PATH"/default_shader.vert", RESOURCE_PATH"/default_shader.frag"));

    if (!shader->get_program()) {
        std::cout << "Failed to create shader" << std::endl;
        return false;
    }

    pipeline.shader = std::move(shader);
    return true;
}

void SpriteBatch::begin() {
    glBindVertexArray(pipeline.vao);
    pipeline.instances.clear();
    pipeline.texture_slots.clear();
}
void SpriteBatch::draw_texture(Texture2D *texture, Vector2 position, Vector2 scale,
                               float rotation, Vector2 origin, Color color, Rect sourceRect) {

    if (pipeline.instances.size() >= pipeline.MAX_INSTANCES) {
        flush();
    }

    auto it = std::find(pipeline.texture_slots.begin(), pipeline.texture_slots.end(), texture);


    if (it == pipeline.texture_slots.end() && pipeline.texture_slots.size() >= pipeline.MAX_TEXTURE_SLOTS) {
        flush();
        pipeline.texture_slots.clear();
    }

    it = std::find(pipeline.texture_slots.begin(), pipeline.texture_slots.end(), texture);

    float srcW = (sourceRect.width > 0) ? sourceRect.width : (float)texture->get_width();
    float srcH = (sourceRect.height > 0) ? sourceRect.height : (float)texture->get_height();

    float pivotX = (origin.x / srcW) - 0.5f;
    float pivotY = (origin.y / srcH) - 0.5f;

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(position.x, position.y, 0.0f));
    model = glm::translate(model, glm::vec3(pivotX * srcW * scale.x, pivotY * srcH * scale.y, 0.0f));
    model = glm::rotate(model, glm::radians(rotation), glm::vec3(0, 0, 1));
    model = glm::scale(model, glm::vec3(scale.x * srcW, scale.y * srcH, 1.0f));
    model = glm::translate(model, glm::vec3(-pivotX, -pivotY, 0.0f));

    InstanceData instance;
    instance.mvp = pipeline.projection * model;
    instance.color[0] = color.r;
    instance.color[1] = color.g;
    instance.color[2] = color.b;
    instance.color[3] = color.a;

    float tex_w = (float)texture->get_width();
    float tex_h = (float)texture->get_height();

    instance.region.x = sourceRect.x / tex_w;
    instance.region.y = sourceRect.y / tex_h;
    instance.region.z = srcW / tex_w;
    instance.region.w = srcH / tex_h;


    if (it != pipeline.texture_slots.end()) {
        instance.texIndex = it - pipeline.texture_slots.begin();
    } else {
        pipeline.texture_slots.push_back(texture);
        instance.texIndex = pipeline.texture_slots.size() - 1;
    }

    pipeline.instances.push_back(instance);
}



void SpriteBatch::flush() const {
    end();
}

void SpriteBatch::end() const {
    if (pipeline.instances.empty()) return;

    glBindBuffer(GL_ARRAY_BUFFER, pipeline.instance_vbo);
    glBufferData(GL_ARRAY_BUFFER, pipeline.MAX_INSTANCES * sizeof(InstanceData), nullptr, GL_STREAM_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, pipeline.instances.size() * sizeof(InstanceData), pipeline.instances.data());

    for (int i = 0; i < pipeline.texture_slots.size(); i++) {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, pipeline.texture_slots[i]->get_texture());
    }

    pipeline.shader->use();


    glDrawElementsInstanced(
        GL_TRIANGLES,
        6,
        GL_UNSIGNED_INT,
        nullptr,
        pipeline.instances.size()
    );
}

void SpriteBatch::set_projection(const glm::mat<4,4,float> &projection) {
    pipeline.projection = projection;

}

SpriteBatch::~SpriteBatch() {
    glDeleteBuffers(1, &pipeline.vbo);
    glDeleteBuffers(1, &pipeline.ebo);
    glDeleteBuffers(1, &pipeline.instance_vbo);
    glDeleteVertexArrays(1, &pipeline.vao);
}
