//
// Created by jhone on 08/08/2025.
//

#ifndef MESH_H
#define MESH_H

#include <vector>

#include "glad/glad.h"
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"

/**
 * Represents a standard vertex with position, color, and texture coordinates.
 */
struct Vertex {
    glm::vec3 position;
    glm::vec4 color;
    glm::vec2 texcoord;
};

/**
 * Describes a custom vertex attribute layout for use with OpenGL shaders.
 */
struct VertexAttribPointer {
    GLint size;
    GLenum type;
    GLboolean normalized;
    GLsizei stride;
    const void *pointer;
};

/**
 * Mesh is a utility class for managing vertex data used in OpenGL rendering.
 *
 * It supports both default vertex layouts (position, color, texcoord) and fully
 * customizable layouts via manual attribute definitions and raw vertex buffers.
 *
 * This class is useful for rendering 2D and 3D geometry using either fixed shaders
 * with a known layout or custom shaders with advanced attribute configurations.
 *
 * Features:
 * - Add and manage default-format vertices
 * - Use raw vertex buffers for custom attributes
 * - Define custom vertex attribute pointers
 * - Add index buffers for indexed drawing
 * - Switch between default and custom rendering pipelines
 *
 * NOTE: When using custom vertex attributes, it's your responsibility to ensure
 * compatibility with the shader being used.
 */
class Mesh {
    std::vector<Vertex> vertices;
    std::vector<GLfloat> custom_vertices;
    std::vector<VertexAttribPointer> custom_attrib_pointers;
    std::vector<GLuint> custom_indices;
    bool override_default_vertices{false};

public:
    Mesh() = default;

    ~Mesh() = default;

    /**
     * Adds a single default-format vertex (position, color, texcoord) to the mesh.
     *
     * @param vertex The vertex to add.
     */
    void add_vertex(const Vertex &vertex);

    /**
     * Adds custom vertex data for use with custom shaders.
     *
     * NOTE: You must also configure custom attributes using `add_custom_vertex_attrib_pointer()`.
     *
     * @param vertices Flat array of custom vertex data (e.g., float values).
     */
    void add_custom_vertex(std::vector<GLfloat> &vertices);

    /**
     * Replaces all previously added default-format vertices with a new list.
     *
     * @param vertices A list of default-format vertices.
     */
    void add_vertices(const std::vector<Vertex> &vertices);

    /**
     * Adds a custom vertex attribute pointer definition to the mesh.
     *
     * Use this when providing your own vertex data layout (e.g., for custom shaders).
     *
     * @param attrib_pointer  Attribute pointer configuration.
     * @param override_default If true, disables the default attributes (position, color, texcoord).
     *                         Set to true only if your shader does not rely on default attributes.
     */
    void add_custom_vertex_attrib_pointer(const VertexAttribPointer &attrib_pointer, bool override_default = false);

    /**
     * Adds custom index data used for drawing the mesh with indexed rendering (e.g., GL_TRIANGLES).
     *
     * @param indices A list of vertex indices.
     */
    void add_custom_indices(std::vector<GLuint> indices);

    /**
     * Returns all default-format vertices.
     *
     * NOTE: Will return an empty list if using custom vertex data.
     *
     * @return Reference to the internal vertex list.
     */
    [[nodiscard]] const std::vector<Vertex> &get_vertices() const;

    /**
     * Returns all custom vertex attribute pointers.
     *
     * @return Reference to the internal list of custom attribute configurations.
     */
    [[nodiscard]] const  std::vector<VertexAttribPointer> &get_custom_vertex_attrib_pointers() const;
    [[nodiscard]] const  std::vector<GLuint> &get_custom_indices() const;
    [[nodiscard]] const  std::vector<GLfloat> &get_custom_vertices() const;

    void clear_all();




};

#endif // MESH_H
