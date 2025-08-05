//
// Created by jhone on 05/08/2025.
//

#include "Mesh.h"

#include <cassert>
#include <stdexcept>

namespace EngineGL {
    Mesh::~Mesh() {
        if (vbo_id != 0)
            glDeleteBuffers(1, &vbo_id);
        if (ebo_id != 0)
            glDeleteBuffers(1, &ebo_id);
        if (vao_id != 0)
            glDeleteVertexArrays(1, &vao_id);
    }

    void Mesh::set_vertices(const GLfloat *data, const size_t size) {
        if (!data || size == 0) throw std::runtime_error("Invalid vertices");
        vertices.assign(data, data + size);
    }

    void Mesh::set_indices(const GLuint *data, size_t size) {
        if (!data || size == 0) throw std::runtime_error("Invalid indices");
        indices.assign(data, data + size);
    }


    void Mesh::create_buffer(const GLenum type) {
        if (this->vao_id == 0) {
            glGenVertexArrays(1, &this->vao_id);
        }

        glBindVertexArray(this->vao_id);

        if (type == GL_ARRAY_BUFFER) {
            glGenBuffers(1, &this->vbo_id);
            glBindBuffer(type, this->vbo_id);

        }else if (type == GL_ELEMENT_ARRAY_BUFFER) {
            glGenBuffers(1, &this->ebo_id);
            glBindBuffer(type, this->ebo_id);
        }
    }

    void Mesh::bind() const {
        assert(this->vao_id != 0 && "vao_id cannot be 0 when binding");
        glBindVertexArray(this->vao_id);

    }

    void Mesh::unbind() {
        glBindVertexArray(0);
    }

    void Mesh::define_buffer_data(const GLenum target, const GLsizeiptr size, const void *data, const GLenum usage) {
        glBufferData(target, size, data, usage);
    }

    void Mesh::define_buffer_data(const GLenum target, const GLsizeiptr size, const void *data) {
        define_buffer_data(target, size, data, GL_STATIC_DRAW);
    }

    void Mesh::set_vertex_attrib_pointer(const GLuint index, const GLint size, GLenum type, GLboolean normalized, GLsizei stride,
        const void *pointer) {

        glEnableVertexAttribArray(index);
        glVertexAttribPointer(index, size, type, normalized, stride, pointer);
    }
} // EngineGL