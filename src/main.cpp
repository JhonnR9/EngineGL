#include <iostream>
#include <fstream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include "glm/mat4x4.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image/stb_image.h>

#include "app.h"

struct Vertex {
    glm::vec2 pos;
    glm::vec2 texCoord;
};

Vertex vertices[4] = {
    {{-1.f, 1.f}, { 0.0f, 0.0f,}},
    {{ 1.f, 1.f}, { 1.0f, 0.0f,}},
    {{ 1.f, -1.f}, {1.f,  1.0f}},
    {{-1.f, -1.f}, {0.0f, 1.0f}}
};

static constexpr GLuint indices[6] = {
    0, 1, 2,
    2, 3, 0
};

std::string load_source(const std::string &file_path) {
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


[[nodiscard]] GLuint load_texture(const char *path, int &width, int &height) {
    int channels;
    uint8_t* data = stbi_load(path, &width, &height, &channels, 4); // 4 RGBA

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);

    return texture;
}

GLuint compile_shader(const GLenum type, const std::string &source) {
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

int main() {
    App& app = App::getInstance();
    app.init();

    // Vertex Array Object
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Vertex Buffer Object
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Vertex Elemental buffer
    GLuint ebo;
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


    const std::string vertex_src = load_source(RESOURCE_PATH "/triangle.vert");
    const std::string fragment_src = load_source(RESOURCE_PATH "/triangle.frag");

    const GLuint vertex_shader = compile_shader(GL_VERTEX_SHADER, vertex_src);
    const GLuint fragment_shader = compile_shader(GL_FRAGMENT_SHADER, fragment_src);

    const GLuint program = glCreateProgram();
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
    glUseProgram(program);

    const GLint mvp_location = glGetUniformLocation(program, "MVP");
    const GLint texture_location = glGetUniformLocation(program, "texture1");
    glUniform1i(texture_location,0);

    // position x and y  (2 floats)
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), static_cast<void *>(nullptr));

    // texCoords (2 floats)
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void *>(offsetof(Vertex, texCoord)));
    glEnableVertexAttribArray(1);

    int text_width, text_height;
    const GLuint texture =  load_texture(RESOURCE_PATH"/texture.jpg", text_width, text_height);


    while (!glfwWindowShouldClose(app.get_window())) {
        int width, height;
        glfwGetFramebufferSize(app.get_window(), &width, &height);
        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0.1, 0.1, 0.14, 1.0);

        const float texture_aspect = static_cast<float>(text_width) / static_cast<float>(text_height);
        glm::mat4 model(1);

        model = glm::rotate(model, glm::radians(90.f), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, glm::vec3(texture_aspect, 1.0f, 1.0f));

        const float aspect = static_cast<float>(width) / static_cast<float>(height);

        glm::mat4x4 projection;
        if (aspect >= 1.0f) {
            projection = glm::ortho(-aspect, aspect, -1.0f, 1.0f);
        } else {
            projection = glm::ortho(-1.0f, 1.0f, -1.0f / aspect, 1.0f / aspect);
        }

        glm::mat4x4 view(1.0f);

        glm::mat4 mvp = projection * view * model;

        glUniformMatrix4fv(mvp_location, 1, GL_FALSE, glm::value_ptr(mvp));

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

        glfwSwapBuffers(app.get_window());
        glfwPollEvents();
    }
    glDeleteTextures(1, &texture);
    glDeleteProgram(program);
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &ebo);

    glfwDestroyWindow(app.get_window());
    glfwTerminate();
    return 0;
}