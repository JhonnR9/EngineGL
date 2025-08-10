// Created by Jhone on 09/08/2025.

#ifndef SPRITEBATCH_H
#define SPRITEBATCH_H

#include <vector>
#include "Drawable.h"
#include "Mesh.h"

/**
 * The SpriteBatch class is designed for efficient rendering of multiple sprites in a single draw call.
 * It uses OpenGL buffers (VBO, EBO, VAO) to batch multiple meshes together before sending them to the GPU.
 * The class schedules mesh draws, accumulates vertex data in CPU memory, and then flushes the accumulated data
 * to the GPU in bulk, ensuring fewer draw calls and improving rendering performance.
 *
 * Key features:
 * - Efficient batching of mesh data.
 * - Minimizes draw calls by accumulating vertex data until a batch threshold is met.
 * - Uses dynamic buffer management to ensure optimal memory usage on the GPU.
 *
 * This approach is ideal for 2D sprite rendering where large numbers of sprites need to be drawn simultaneously.
 */
class SpriteBatch {
    // Current vertex buffer for rendering
    std::vector<Vertex> vertices;
    // OpenGL buffers for uploading to GPU
    GLuint VAO{0}, VBO{0}, EBO{0};

    // Memory allocation control based on buffer usage limitsx'
    unsigned int max_vbo_alloc = 1024 * 1024; // 1MB for vertex buffer
    unsigned int max_ebo_alloc = 256 * 1024;  // 256KB for element buffer
    unsigned int current_vbo_alloc {0};       // Current VBO allocation on the GPU
    unsigned int current_ebo_alloc {0};       // Current EBO allocation on the GPU

    /**
     * Initializes the necessary OpenGL buffers for the SpriteBatch:
     * - Generates and binds the Vertex Array Object (VAO).
     * - Generates and binds the Vertex Buffer Object (VBO), allocating GPU memory
     *   based on the logical draw call limit with a 50% safety margin to prevent overflow.
     * - Generates and binds the Element Buffer Object (EBO), similarly allocating memory
     *   with a 50% safety margin.
     *
     * This setup prepares the buffers for dynamic drawing operations.
     */
    void init();

    /**
     * Schedules a mesh for drawing by adding it to the CPU buffer.
     * The actual draw call will occur once a minimum buffer size is reached or at the end of the frame.
     *
     * @param mesh The Mesh object containing all vertices and attributes required to update the buffer.
     */
    void schedule_mesh_draw(const Mesh& mesh);

    /**
     * Executes the draw call with all accumulated buffer data.
     */
    void make_draw_call();

public:
    SpriteBatch() = default;
    ~SpriteBatch() = default;

    /**
     * Schedules a draw for a Drawable object and prepares the necessary buffers on the CPU.
     * The draw will only occur when there is enough accumulated data to justify a draw call.
     *
     * @param target The object to be drawn, containing vertex data, buffers, and points.
     */
    void draw(Drawable &target);

    /**
     * This function should be called at the end of each frame.
     * Its purpose is to transfer any remaining data to the GPU before the next frame begins.
     * It performs a final draw call to flush accumulated data.
     */
    void flush();
};

#endif // SPRITEBATCH_H
