//
// Created by jhone on 08/08/2025.
//

#ifndef MESH_INFO_H
#define MESH_INFO_H

#include <vector>

#include "glad/glad.h"
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"

struct Vertex {
    glm::vec3 position;
    glm::vec3 color;
    glm::vec2 texcoord;
};

struct GLBuffer {
    GLuint id{};
    GLenum target{};
};
class MeshInfo {
    std::vector<GLBuffer> buffers;
    std::vector<Vertex> vertices;

public:
    MeshInfo() =default;
    ~MeshInfo() =default;

    void add_buffer(const GLBuffer& buffer);
    void add_vertex(const Vertex& vertex);
    void add_vertices(const std::vector<Vertex>& vertices);

    [[nodiscard]] const std::vector<GLBuffer> &get_buffers() const;
    [[nodiscard]] const std::vector<Vertex> &get_vertices() const;

};


#endif //MESH_INFO_H
