//
// Created by jhone on 04/08/2025.
//

#include "Shader.h"
#include <fstream>
#include <iostream>
#include <utility>

namespace EngineGL {

    Shader::Shader( std::string vertex_shader_path,  std::string fragment_shader_path) {

        vertex_shader_source = load_source(std::move(vertex_shader_path));
        frag_shader_source = load_source(std::move(fragment_shader_path));

    }

    Shader::~Shader() {
        if (program_id != 0) {
            glDeleteProgram(program_id);
        }

    }

    void Shader::bind() const {
        glUseProgram(program_id);
    }

    void Shader::unbind() {
        glUseProgram(0);
    }


    void Shader::compile_shaders() {
        const GLuint vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);
        const GLchar* vertex_shader_text = vertex_shader_source.c_str();

        glShaderSource(vertex_shader_id, 1, &vertex_shader_text, NULL);
        glCompileShader(vertex_shader_id);
        check_shader_compile(vertex_shader_id);

        const GLuint frag_shader_id = glCreateShader(GL_FRAGMENT_SHADER);
        const GLchar* frag_shader_text = frag_shader_source.c_str();

        glShaderSource(frag_shader_id, 1, &frag_shader_text, NULL);
        glCompileShader(frag_shader_id);
        check_shader_compile(frag_shader_id);

        link_program(vertex_shader_id, frag_shader_id);
        check_program_link(program_id);

        glDeleteShader(vertex_shader_id);
        glDeleteShader(frag_shader_id);
    }

    void Shader::link_program(GLuint vertex_shader_id, GLuint frag_shader_id) {
        if (program_id == 0) {
            program_id = glCreateProgram();

        }
        glAttachShader(program_id, vertex_shader_id);
        glAttachShader(program_id, frag_shader_id);
        glLinkProgram(program_id);

    }

    void Shader::check_shader_compile(GLuint shader) {
        GLint success;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            char infoLog[512];
            glGetShaderInfoLog(shader, 512, NULL, infoLog);
            fprintf(stderr, "Shader compile error:\n%s\n", infoLog);
        }
    }

    void Shader::check_program_link(GLuint program) {
        GLint success;
        glGetProgramiv(program, GL_LINK_STATUS, &success);
        if (!success) {
            char infoLog[512];
            glGetProgramInfoLog(program, 512, NULL, infoLog);
            fprintf(stderr, "Program link error:\n%s\n", infoLog);
        }
    }

    std::string Shader::load_source( std::string file_path) {
        std::ifstream file;
        file.open(file_path, std::ios::in);

        if (!file.is_open()) {
            std::cerr << "Failed to open " << file_path << std::endl;
            return "null";
        }
        std::string line;
        std::string source;
        while (std::getline(file, line)) {
            source += line + "\n";
        }

        if (source.empty()) {
            throw std::runtime_error("Shader source is empty: " + file_path);
        }

        return source;

    }
} // EngineGL
