#include "sprite_batch.h"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include "main/app.h"
#include "utils/vector2.h"

SpriteBatch::SpriteBatch(OrthographicCamera *camera) {
    if (!create_default_shader()) {
        std::cerr << "Error creating default shader" << std::endl;
    }

    setup_buffers();

    if (!camera) {
        std::cerr << "Error: Camera pointer canot be null in SpriteBatch!" << std::endl;
    } else {
        pipeline.shader->set_mat4("uViewProjection", camera->get_view_projection());
    }


    pipeline.camera = camera;

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
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

    // aTranslation (vec2)
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(InstanceData), (void *) offsetof(InstanceData, position));
    glVertexAttribDivisor(3, 1);

    // aOrigin (vec2)
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, sizeof(InstanceData), (void *) offsetof(InstanceData, origin));
    glVertexAttribDivisor(4, 1);

    // aRotation (float)
    glEnableVertexAttribArray(5);
    glVertexAttribPointer(5, 1, GL_FLOAT, GL_FALSE, sizeof(InstanceData), (void *) offsetof(InstanceData, rotation));
    glVertexAttribDivisor(5, 1);

    // aScale (vec2)
    glEnableVertexAttribArray(6);
    glVertexAttribPointer(6, 2, GL_FLOAT, GL_FALSE, sizeof(InstanceData), (void *) offsetof(InstanceData, scale));
    glVertexAttribDivisor(6, 1);

    // aInstanceColor (vec4)
    glEnableVertexAttribArray(7);
    glVertexAttribPointer(7, 4, GL_FLOAT, GL_FALSE, sizeof(InstanceData), (void *) offsetof(InstanceData, color));
    glVertexAttribDivisor(7, 1);

    // aRegion (vec4)
    glEnableVertexAttribArray(8);
    glVertexAttribPointer(8, 4, GL_FLOAT, GL_FALSE, sizeof(InstanceData), (void *) offsetof(InstanceData, region));
    glVertexAttribDivisor(8, 1);

    // aTexIndex (float)
    glEnableVertexAttribArray(9);
    glVertexAttribPointer(9, 1, GL_FLOAT, GL_FALSE, sizeof(InstanceData), (void *) offsetof(InstanceData, tex_index));
    glVertexAttribDivisor(9, 1);

    // aFlip (int)
    glEnableVertexAttribArray(10);
    glVertexAttribIPointer(10, 1, GL_INT, sizeof(InstanceData), (void *) offsetof(InstanceData, flip));
    glVertexAttribDivisor(10, 1);

    // aZindex (float)
    glEnableVertexAttribArray(11);
    glVertexAttribPointer(11, 1, GL_FLOAT,GL_FALSE, sizeof(InstanceData), (void *) offsetof(InstanceData, z_index));
    glVertexAttribDivisor(11, 1);

    // aShapyType (int)
    glEnableVertexAttribArray(12);
    glVertexAttribIPointer(12, 1,GL_INT, sizeof(InstanceData), (void *) offsetof(InstanceData, shape_type));
    glVertexAttribDivisor(12, 1);

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

bool SpriteBatch::create_default_shader() {
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
    pipeline.shader->use();

    if (pipeline.camera) {
        pipeline.shader->use();
        pipeline.shader->set_mat4("uViewProjection", pipeline.camera->get_view_projection());
    }


    glBindVertexArray(pipeline.vao);
    pipeline.instances.clear();
    pipeline.instances.reserve(pipeline.MAX_INSTANCES);
    pipeline.texture_slots.clear();

    glClearColor(0.043f, 0.067f, 0.090f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void SpriteBatch::draw_texture(Texture2D *texture, Vector2 position, Vector2 scale,
                               float rotation, Vector2 origin, Color color, Rect sourceRect, bool flip_x, bool flip_y,
                               float z_index) {
    if (pipeline.instances.size() >= pipeline.MAX_INSTANCES) {
        flush();
    }

    InstanceData instance{};

    auto it = std::find(pipeline.texture_slots.begin(), pipeline.texture_slots.end(), texture);

    if (it == pipeline.texture_slots.end()) {
        if (pipeline.texture_slots.size() >= pipeline.MAX_TEXTURE_SLOTS) {
            flush();
            pipeline.texture_slots.clear();
        }

        pipeline.texture_slots.push_back(texture);
        instance.tex_index = pipeline.texture_slots.size() - 1;
    } else {
        instance.tex_index = it - pipeline.texture_slots.begin();
    }




    float srcW = (sourceRect.width > 0) ? sourceRect.width : (float) texture->get_width();
    float srcH = (sourceRect.height > 0) ? sourceRect.height : (float) texture->get_height();

    float pivotX = (origin.x / srcW) - 0.5f;
    float pivotY = (origin.y / srcH) - 0.5f;


    instance.position = glm::vec2(position.x, position.y);
    instance.origin = glm::vec2(pivotX, pivotY);
    instance.scale = glm::vec2(srcW * scale.x, srcH * scale.y);
    instance.rotation = rotation;

    instance.color[0] = color.r;
    instance.color[1] = color.g;
    instance.color[2] = color.b;
    instance.color[3] = color.a;

    float tex_w = (float) texture->get_width();
    float tex_h = (float) texture->get_height();

    instance.region.x = sourceRect.x / tex_w;
    instance.region.y = sourceRect.y / tex_h;
    instance.region.z = srcW / tex_w;
    instance.region.w = srcH / tex_h;

    instance.flip = (flip_x ? 1 : 0) | (flip_y ? 2 : 0);
    instance.z_index = z_index;


    pipeline.instances.push_back(instance);
}

void SpriteBatch::draw_shape(Rect rect, Color color, Vector2 origin, float rotation, ShapeType type, float z_index) {
    if (rect.width <= 0.0f || rect.height <= 0.0f) {
        return;
    }

    if (pipeline.instances.size() >= pipeline.MAX_INSTANCES) {
        flush();
    }

    InstanceData instance{};

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

    instance.shape_type = static_cast<int>(type);
    instance.z_index = z_index;

    pipeline.instances.push_back(instance);
}

void SpriteBatch::draw_rect(Rect rect, Color color, Vector2 origin, float rotation, float z_index) {
    draw_shape(rect, color, origin, rotation, ShapeType::Rectangle, z_index);
}

void SpriteBatch::draw_ellipse(Ellipse ellipse, Color color, float z_index) {
    Rect rect;
    rect.x = ellipse.cx;
    rect.y = ellipse.cy;
    rect.width = ellipse.rx * 2.f;
    rect.height = ellipse.ry * 2.f;

    draw_shape(rect, color, Vector2(0.5f, 0.5f), 0.0f, ShapeType::Ellipse, z_index);
}

void SpriteBatch::draw_line(Vector2 start, Vector2 end, float thickness, Color color, float z_index) {
    Vector2 dir;
    dir.x = end.x - start.x;
    dir.y = end.y - start.y;

    float length = sqrt(dir.x * dir.x + dir.y * dir.y);

    float angle = atan2(dir.y, dir.x);

    Rect rect;
    rect.x = start.x;
    rect.y = start.y;
    rect.width = length;
    rect.height = thickness;

    draw_shape(
        rect,
        color,
        Vector2(0.0f, 0.5f),
        glm::degrees(angle),
        ShapeType::Line,
        z_index
    );
}

void SpriteBatch::draw_texture(Texture2D *texture, Vector2 position, float z_index) {
    draw_texture(
        texture,
        position,
        Vector2{1.0f, 1.0f},
        0.0f,
        Vector2{0.0f, 0.0f},
        Color{1.0f, 1.0f, 1.0f, 1.0f},
        Rect{0, 0, (float) texture->get_width(), (float) texture->get_height()},
        false,
        false,
        z_index
    );
}

void SpriteBatch::draw_texture(Texture2D *texture, Vector2 position, Vector2 scale, float z_index) {
    draw_texture(
        texture,
        position,
        scale,
        0.0f,
        Vector2{0.0f, 0.0f},
        Color{1.0f, 1.0f, 1.0f, 1.0f},
        Rect{0, 0, (float) texture->get_width(), (float) texture->get_height()},
        false,
        false,
        z_index
    );
}

void SpriteBatch::draw_texture(Texture2D *texture, Vector2 position, Vector2 scale, float rotation, float z_index) {
    draw_texture(
        texture,
        position,
        scale,
        rotation,
        Vector2{0.0f, 0.0f},
        Color{1.0f, 1.0f, 1.0f, 1.0f},
        Rect{0, 0, (float) texture->get_width(), (float) texture->get_height()},
        false,
        false,
        z_index
    );
}

void SpriteBatch::draw_texture(Texture2D *texture, Vector2 position, Vector2 scale, float rotation, Color color,
                               float z_index) {
    draw_texture(
        texture,
        position,
        scale,
        rotation,
        Vector2{0.0f, 0.0f},
        color,
        Rect{0, 0, (float) texture->get_width(), (float) texture->get_height()}
        ,
        false,
        false,
        z_index);
}

void SpriteBatch::draw_text(Font &font, const std::string &text, Vector2 position, float scale,
                            Color color, float z_index) {
    float x = position.x;

    float baseline = position.y + font.get_ascent() * scale;

    for (char c: text) {
        const Character &ch = font.characters.at(c);

        float xpos = x + ch.bearing.x * scale;
        float ypos = baseline - ch.bearing.y * scale;

        float w = ch.size.x * scale;
        float h = ch.size.y * scale;

        float texW = (float) font.atlas_texture->get_width();
        float texH = (float) font.atlas_texture->get_height();

        Rect src;
        src.x = ch.uv_min.x * texW;
        src.y = ch.uv_min.y * texH;
        src.width  = (ch.uv_max.x - ch.uv_min.x) * texW;
        src.height = (ch.uv_max.y - ch.uv_min.y) * texH;

        draw_texture(
            font.atlas_texture.get(),
            Vector2{xpos, ypos},
            Vector2{w / src.width, h / src.height},
            0.0f,
            Vector2{0.0f, 0.0f},
            color,
            src,
            false,
            false,
            z_index
        );

        x += ch.advance * scale;
    }
}



void SpriteBatch::flush() {
    end();
}

void SpriteBatch::end() {
    if (pipeline.instances.empty()) return;

    std::ranges::sort(pipeline.instances,
                      [](const InstanceData &a, const InstanceData &b) {
                          if (a.z_index == b.z_index)
                              return a.tex_index < b.tex_index;
                          return a.z_index < b.z_index;
                      });


    pipeline.shader->use();


    glBindBuffer(GL_ARRAY_BUFFER, pipeline.instance_vbo);
    glBufferData(GL_ARRAY_BUFFER, pipeline.MAX_INSTANCES * sizeof(InstanceData), nullptr, GL_STREAM_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, pipeline.instances.size() * sizeof(InstanceData), pipeline.instances.data());

    for (int i = 0; i < pipeline.texture_slots.size(); i++) {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, pipeline.texture_slots[i]->get_texture());
    }

    glDrawElementsInstanced(
        GL_TRIANGLES,
        6,
        GL_UNSIGNED_INT,
        nullptr,
        pipeline.instances.size()
    );
}

SpriteBatch::~SpriteBatch() {
    glDeleteBuffers(1, &pipeline.vbo);
    glDeleteBuffers(1, &pipeline.ebo);
    glDeleteBuffers(1, &pipeline.instance_vbo);
    glDeleteVertexArrays(1, &pipeline.vao);
}
