//
// Created by jhone on 09/08/2025.
//

#ifndef SPRITEBATCH_H
#define SPRITEBATCH_H
#include <vector>

#include "Drawable.h"
#include "MeshInfo.h"

class SpriteBatch {
    std::vector<MeshInfo> meshes;

    // the current batch for rendering
    std::vector<Vertex> vertices;
    bool has_drawn_this_frame = false;

    GLuint VAO{0}, VBO{0}, EBO{0};
    unsigned int vbo_men_max_alloc = 2048 * 1024; // 2MB
    unsigned int ebo_mem_max_alloc = 256 * 1024;  // 256KB

    unsigned int vbo_gpu_current_alloc {0};
    unsigned int ebo_gpu_current_alloc {0};

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
    void init();

    void schedule_mesh_draw(const MeshInfo& mesh);
public:
    SpriteBatch() = default;

    ~SpriteBatch() = default;

    /**
     * This function only schedules the drawing and prepares the buffer in the CPU,
     * the drawing only occurs when we have enough data to compensate for the draw call
     * @param target object that will be drawn with information about Vertices, buffers and points
     */
    void draw(Drawable &target);

    void make_draw_call();
};

#endif //SPRITEBATCH_H
