//
// Created by jhone on 09/08/2025.
//

#include "SpriteBatch.h"

#include <iostream>
#include <ostream>

/**
 * Creates and initializes the necessary OpenGL buffers for the SpriteBatch:
 * - Generates and binds a Vertex Array Object (VAO).
 * - Generates and binds a Vertex Buffer Object (VBO), allocating GPU memory
 *   based on the logical draw call limit plus a 50% safety margin to handle overflow.
 * - Generates and binds an Element Buffer Object (EBO), similarly allocating
 *   memory with a 50% margin for safety.
 *
 * This setup prepares the buffers for dynamic drawing operations.
 */
void SpriteBatch::init() {
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // Allocate GPU memory based on the logical draw call limit (vbo_men_max_alloc),
    // plus 50% extra as a safety margin to ensure the buffer can handle slight overflows.
    glBufferData(GL_ARRAY_BUFFER, vbo_men_max_alloc + (vbo_men_max_alloc / 2), nullptr, GL_DYNAMIC_DRAW);

    // Same, but now ebo (ebo_mem_max_alloc),
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, ebo_mem_max_alloc + (ebo_mem_max_alloc / 2), nullptr, GL_DYNAMIC_DRAW);
}


void SpriteBatch::schedule_mesh_draw(const Mesh &mesh) {
    const unsigned int data_size = mesh.get_vertices().size() * sizeof(Vertex);
    vbo_gpu_current_alloc += data_size;
    vertices.insert( vertices.end(),mesh.get_vertices().begin(), mesh.get_vertices().end());

}

/**
 * This function only schedules the drawing and prepares the buffer in the CPU,
 * the drawing only occurs when we have enough data to compensate for the draw call
 * @param target object that will be drawn with information about Vertices, buffers and points
 */
void SpriteBatch::draw(Drawable& target) {
    assert(!target.get_mesh_info().get_vertices().empty() && "Vertices could not be empty");

    schedule_mesh_draw(target.get_mesh_info());

    if (vbo_gpu_current_alloc >= vbo_men_max_alloc ||
        ebo_gpu_current_alloc >= ebo_mem_max_alloc) {
        make_draw_call();

    }

}

void SpriteBatch::make_draw_call() {
    // TODO implementation of draw yourself
}
