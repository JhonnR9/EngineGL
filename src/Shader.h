//
// Created by jhone on 04/08/2025.
//

#ifndef SHADER_H
#define SHADER_H

#include <string>

#include "glad/glad.h"

namespace EngineGL {
    class Shader {
        GLuint vao_id{0}, vbo_id{0}, program_id{0}, ebo_id{0};
        std::string vertex_shader_source, frag_shader_source;

        void link_program(GLuint vertex_shader_id, GLuint frag_shader_id);
        static void check_shader_compile(GLuint shader);
        static void check_program_link(GLuint program);

    public:
        [[nodiscard]] GLuint get_program_id() const { return program_id; }

        [[nodiscard]] GLuint get_vao_id() const {return vao_id;}

        static std::string load_source( std::string file_path);
        static void define_buffer_data(GLenum target, GLsizeiptr size, const void *data, GLenum usage);
        static void define_buffer_data(GLenum target, GLsizeiptr size, const void *data);
        void create_buffer(GLenum type);
        void compile_shaders();
        Shader( std::string vertex_shader_path,  std::string fragment_shader_path);
        ~Shader();
    };
} // EngineGL

#endif //SHADER_H
