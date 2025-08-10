//
// Created by jhone on 09/08/2025.
//

#include "SpriteBatch.h"
#include <iostream>


void SpriteBatch::init() {
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // Allocate GPU memory based on the logical draw call limit (max_vbo_alloc),
    // plus 50% extra as a safety margin to ensure the buffer can handle slight overflows.
    glBufferData(GL_ARRAY_BUFFER, max_vbo_alloc + (max_vbo_alloc / 2), nullptr, GL_DYNAMIC_DRAW);

    // -------- Set default pointers -------- //
    // position x y z  (3 floats)
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), static_cast<void *>(nullptr));

    // color r g b a (4 floats)
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          reinterpret_cast<void *>(offsetof(Vertex, color)));

    // texCoords (2 floats)
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          reinterpret_cast<void *>(offsetof(Vertex, texcoord)));

    GLuint default_indices[6] = {
        0, 1, 2,
        2, 3, 0
    };

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(default_indices), default_indices, GL_STATIC_DRAW);
}


void SpriteBatch::schedule_mesh_draw(const Mesh &mesh) {
    const unsigned int data_size = mesh.get_vertices().size() * sizeof(Vertex);
    current_vbo_alloc += data_size;

    if (mesh.get_custom_vertices().empty() || mesh.get_custom_vertex_attrib_pointers().empty()) {
        if (mesh.get_vertices().empty()) {
            std::cerr << "Empty vertices!" << std::endl;
            return;
        }
        vertices.insert(vertices.end(), mesh.get_vertices().begin(), mesh.get_vertices().end());
    } else {
        // TODO make custom shaders system
    }
}


SpriteBatch::~SpriteBatch() {
    if (VBO) {
        glDeleteBuffers(1, &VBO);
    }
    if (EBO) {
        glDeleteBuffers(1, &EBO);
    }
    if (VAO) {
        glDeleteVertexArrays(1, &VAO);
    }
}

void SpriteBatch::draw(Drawable &target) {
    assert(!target.get_mesh_info().get_vertices().empty() && "Vertices could not be empty");

    schedule_mesh_draw(target.get_mesh_info());

    if (current_vbo_alloc >= max_vbo_alloc) {
        make_draw_call();
    }
}

void SpriteBatch::flush() {
    make_draw_call();
}

void SpriteBatch::begin() const {
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    default_shader->use();
}

void SpriteBatch::end() {
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    current_vbo_alloc = 0;
}

void SpriteBatch::make_draw_call() {
    glBufferSubData(
        GL_ARRAY_BUFFER, 0,
        static_cast<GLsizeiptr>(vertices.size() * sizeof(Vertex)),
        vertices.data()
    );
    glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(vertices.size()));
    vertices.clear();
}

SpriteBatch::SpriteBatch() {
    const char *vertex_shader_src = "\n"
            "#version 330 core\n"
            "layout(location = 0) in vec3 aPos;\n"
            "layout(location = 1) in vec4 aColor;\n"
            "layout(location = 2) in vec2 aTexCoord;\n"
            " uniform mat4 MVP;\n"
            " out vec4 color;\n"
            " out vec2 texCoord;\n"
            "    void main()\n"
            "    { \n"
            "       gl_Position = MVP * vec4(aPos, 1.0);\n"
            "       color = aColor;\n"
            "       texCoord = aTexCoord;\n"
            "    }\n";

    const char *fragment_shader_src = "\n"
            "#version 330 core\n"
            "in vec2 texCoord;\n"
            "in vec4 color;\n"
            "out vec4 FragColor;\n"
            "uniform sampler2D texture1;\n"
            " void main()\n"
            "  {\n"
            "       FragColor = texture(texture1, texCoord) * color;\n"
            "  }\n";

    default_shader = std::make_unique<Shader>(SourcePath(vertex_shader_src, fragment_shader_src));


}
