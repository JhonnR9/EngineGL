//
// Created by jhone on 09/08/2025.
//

#ifndef SHADER_H
#define SHADER_H

#include <string>
#include "glad/glad.h"
#include "glm/mat4x4.hpp"

struct SourcePath {
    const char *vertex;
    const char *fragment;

    SourcePath(const char *vertex, const char *fragment) : vertex(vertex), fragment(fragment) {
    }
};

class Shader {
    GLuint program{0};

    static std::string load_source(const std::string &file_path);

    static GLuint compile_shader(GLenum type, const std::string &source);
    void create_shader(const char *vertex_src, const char *fragment_src);

public:
    explicit Shader(const SourcePath &source_path);

    void set_mat4(const std::string& name, const glm::mat4& mat) const {
        glUniformMatrix4fv(glGetUniformLocation(program, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }


    Shader(const char *vertex_shader_src, const char *fragment_shader_src);
    void set_int(const std::string& name, int value) const;
    void set_int_array(const std::string& name, int* values, uint32_t count) const;

    ~Shader();

    [[nodiscard]] GLuint get_program() const;

    void use() const;
};

#endif //SHADER_H
