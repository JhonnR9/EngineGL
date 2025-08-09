//
// Created by jhone on 08/08/2025.
//

#include "MeshInfo.h"

void MeshInfo::add_vertices(const std::vector<Vertex>& vertices) {
    this->vertices.assign(vertices.begin(), vertices.end());
}

const std::vector<GLBuffer> & MeshInfo::get_buffers() const {
    return this->buffers;
}

const std::vector<Vertex> & MeshInfo::get_vertices() const {
    return this->vertices;
}


void MeshInfo::add_vertex(const Vertex& vertex) {
    this->vertices.push_back(vertex);
}


void MeshInfo::add_buffer(const GLBuffer& buffer) {
    this->buffers.push_back(buffer);
}
