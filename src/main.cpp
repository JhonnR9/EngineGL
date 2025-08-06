#include <iostream>
#include <fstream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include "glm/mat4x4.hpp"
#include "glm/matrix.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/gtc/type_ptr.inl"



typedef struct Vertex {
    glm::vec2 pos;
    glm::vec3 col;
} Vertex;

static constexpr Vertex vertices[4] = {
    {{-1.f, 1.f}, {1.f, 0.f, 0.f}},
    {{1.f, 1.f}, {0.f, 0.f, 1.f}},
    {{1.f, -1.f}, {0.f, 1.f, 0.f}},
    {{-1.f, -1.f}, {1.f, 1.f, 1.f}}
};

static constexpr GLuint indices[6] = {
    0,1,2,
    2,3,0
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

static void error_callback(int error, const char *description) {
    std::cerr << "GLFW Error: " << description << std::endl;
}

static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
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
    glfwSetErrorCallback(error_callback);

    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(640, 480, "Simple Triangle", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);

    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glfwSwapInterval(1);

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

    const GLuint VAO = compile_shader(GL_VERTEX_SHADER, vertex_src);
    const GLuint fragment_shader = compile_shader(GL_FRAGMENT_SHADER, fragment_src);

    const GLuint program = glCreateProgram();
    glAttachShader(program, VAO);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);

    glUseProgram(program);

    const GLint mvp_location = glGetUniformLocation(program, "MVP");
    const GLint vpos_location = glGetAttribLocation(program, "vPos");
    const GLint vcol_location = glGetAttribLocation(program, "vCol");

    glEnableVertexAttribArray(vpos_location);
    glVertexAttribPointer(
        vpos_location, 2, GL_FLOAT, GL_FALSE,sizeof(Vertex), static_cast<void *>(nullptr)
    );

    glEnableVertexAttribArray(vcol_location);
    glVertexAttribPointer(
        vcol_location, 3, GL_FLOAT, GL_FALSE,sizeof(Vertex), reinterpret_cast<void *>(offsetof(Vertex, col))
    );



    while (!glfwWindowShouldClose(window)) {
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);

        glm::mat4 model(1);
        model = glm::scale(model, glm::vec3(0.3f));
        const auto new_position_y = sin(glfwGetTime());
        model = glm::translate(model, glm::vec3(0.f, new_position_y * .3f, 0.f));
        const float aspect = static_cast<float>(width) / static_cast<float>(height);

        glm::mat4x4  projection;
        if (aspect >= 1.0f) {
            projection = glm::ortho(-aspect, aspect, -1.0f, 1.0f);
        } else {
            projection = glm::ortho(-1.0f, 1.0f, -1.0f / aspect, 1.0f / aspect);
        }

        glm::mat4x4 view(1.0f);
        glm::mat4 mvp = projection * view * model;

        glUniformMatrix4fv(mvp_location, 1, GL_FALSE, glm::value_ptr(mvp));

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteProgram(program);
    glDeleteShader(VAO);
    glDeleteShader(fragment_shader);
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
