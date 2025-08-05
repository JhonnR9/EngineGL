//
// Created by jhone on 04/08/2025.
//

#ifndef SHADER_H
#define SHADER_H

#include <string>

#include "glad/glad.h"

namespace EngineGL {
    class Shader {
        GLuint program_id{0};
        std::string vertex_shader_source, frag_shader_source;

        void link_program(GLuint vertex_shader_id, GLuint frag_shader_id);
        static void check_shader_compile(GLuint shader);
        static void check_program_link(GLuint program);

    public:
        [[nodiscard]] GLuint get_program_id() const { return program_id; }
        static std::string load_source( std::string file_path);
        void compile_shaders();
        Shader( std::string vertex_shader_path,  std::string fragment_shader_path);
        ~Shader();

        void bind() const;
        static void unbind();
    };
} // EngineGL

#endif //SHADER_H
