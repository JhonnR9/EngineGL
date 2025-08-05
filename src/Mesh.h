//
// Created by jhone on 05/08/2025.
//

#ifndef MESH_H
#define MESH_H

#include <vector>
#include <glad/glad.h>

namespace EngineGL {

class Mesh {
    std::vector<GLfloat> vertices;
    std::vector<GLuint> indices;
    GLuint vao_id{0}, vbo_id{0}, ebo_id{0};
public:
    Mesh()=default;
    ~Mesh();

    void set_vertices(const GLfloat * data, size_t size);
    void set_indices(const GLuint * data, size_t size);

    static void define_buffer_data(GLenum target, GLsizeiptr size, const void *data, GLenum usage);
    static void define_buffer_data(GLenum target, GLsizeiptr size, const void *data);
    static void set_vertex_attrib_pointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer);

    void create_buffer(GLenum type);
    void bind() const;
    static void unbind();

    [[nodiscard]] GLuint get_vao_id() const {return vao_id;}

};

} // EngineGL

#endif //MESH_H
