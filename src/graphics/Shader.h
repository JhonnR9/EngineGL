//
// Created by jhone on 09/08/2025.
//

#ifndef SHADER_H
#define SHADER_H

#include <string>
#include "glad/glad.h"

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

    Shader(const char *vertex_shader_src, const char *fragment_shader_src);

    ~Shader();

    [[nodiscard]] GLuint get_program() const;

    void use() const;
};

#endif //SHADER_H
