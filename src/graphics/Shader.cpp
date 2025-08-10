//
// Created by jhone on 09/08/2025.
//

#include "Shader.h"

#include <fstream>
#include <iostream>

std::string Shader::load_source(const std::string &file_path) {
    std::ifstream file(file_path);
    if (!file.is_open()) {
        std::cerr << "Failed to open " << file_path << std::endl;
        return "";
    }
    std::string line, source;
    while (std::getline(file, line)) {
        source += line + '\n';
    }
    return source;
}

GLuint Shader::compile_shader(const GLenum type, const std::string &source) {
    const GLuint shader = glCreateShader(type);
    const char *src = source.c_str();
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);

    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (!status) {
        char log[512];
        glGetShaderInfoLog(shader, 512, nullptr, log);
        std::cerr << "Shader compilation error: " << log << std::endl;
    }

    return shader;
}

void Shader::create_shader(const char *vertex_src, const char *fragment_src) {
    const GLuint vertex_shader = compile_shader(GL_VERTEX_SHADER, vertex_src);
    const GLuint fragment_shader = compile_shader(GL_FRAGMENT_SHADER, fragment_src);

    program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);

    glLinkProgram(program);

    GLint linkStatus;
    glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
    if (!linkStatus) {
        char log[512];
        glGetProgramInfoLog(program, 512, nullptr, log);
        std::cerr << "Shader program link error: " << log << std::endl;
    }

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

}

Shader::Shader(const SourcePath &source_path) {

    const std::string vertex_src = load_source(source_path.vertex);
    const std::string fragment_src = load_source(source_path.fragment);

    create_shader(vertex_src.c_str(), fragment_src.c_str());

}

Shader::Shader(const char *vertex_shader_src, const char *fragment_shader_src) {
    create_shader(vertex_shader_src, fragment_shader_src);
}

Shader::~Shader() {
    glDeleteProgram(program);
}

GLuint Shader::get_program() const {
    if (!program) {
        std::cerr << "Failed to get program" << std::endl;
    }

    return program;
}

void Shader::use() const {
    glUseProgram(program);
}
