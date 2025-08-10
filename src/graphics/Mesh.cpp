//
// Created by jhone on 08/08/2025.
//

#include "Mesh.h"

void Mesh::add_vertices(const std::vector<Vertex>& vertices) {
    this->vertices.assign(vertices.begin(), vertices.end());
}

void Mesh::add_custom_vertex_attrib_pointer(const VertexAttribPointer &attrib_pointer, const bool override_default) {
    custom_attrib_pointers.push_back(attrib_pointer);
    override_default_vertices = override_default;
}

void Mesh::add_custom_indices(std::vector<GLuint> indices) {
}

const std::vector<Vertex> & Mesh::get_vertices() const {
    return this->vertices;
}

const std::vector<VertexAttribPointer>& Mesh::get_custom_vertex_attrib_pointers() const {
    return custom_attrib_pointers;
}

void Mesh::clear_all() {
    this->vertices.clear();
    this->custom_attrib_pointers.clear();
    this->override_default_vertices = false;
    this->custom_indices.clear();
}

const std::vector<GLuint>& Mesh::get_custom_indices() const {
    return custom_indices;
}

const std::vector<GLfloat>& Mesh::get_custom_vertices() const {
    return custom_vertices;
}


void Mesh::add_vertex(const Vertex& vertex) {
    this->vertices.push_back(vertex);
}

void Mesh::add_custom_vertex(std::vector<GLfloat>& vertices) {
    assert(custom_vertices.empty() && "custom_vertices is empty");
    custom_vertices.assign(vertices.begin(), vertices.end());
}


