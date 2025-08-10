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

    // Do the same for the EBO (max_ebo_alloc), with an additional 50% margin.
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, max_ebo_alloc + (max_ebo_alloc / 2), nullptr, GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);

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

    }
}


void SpriteBatch::draw(Drawable &target) {
    assert(!target.get_mesh_info().get_vertices().empty() && "Vertices could not be empty");

    schedule_mesh_draw(target.get_mesh_info());

    if (current_vbo_alloc >= max_vbo_alloc ||
        current_ebo_alloc >= max_ebo_alloc) {
        make_draw_call();
    }
}

void SpriteBatch::flush() {
    make_draw_call();
}

void SpriteBatch::make_draw_call() {
    // TODO implementation of draw yourself
}
