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

static bool is_fullscreen = false;
static int windowed_xpos = 100, windowed_ypos = 100;
static int windowed_width = 640, windowed_height = 480;
glm::vec2 camera_position;


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

static void error_callback(int error, const char *description) {
    std::cerr << "GLFW Error: " << description << std::endl;
}

static void key_callback(GLFWwindow *win, const int key, int scancode,const int action, int mods) {
    if (key == GLFW_KEY_LEFT && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
        camera_position.x += 0.01f;
    }else if (key == GLFW_KEY_RIGHT && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
        camera_position.x -= 0.01f;
    }else if (key == GLFW_KEY_DOWN && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
        camera_position.y += 0.01f;
    }else if (key == GLFW_KEY_UP && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
        camera_position.y -= 0.01f;
    }

    if ((key == GLFW_KEY_F11 || key==GLFW_KEY_ESCAPE) && action == GLFW_PRESS) {

        if ( key == GLFW_KEY_ESCAPE) {
            is_fullscreen = false;
        }else {
            is_fullscreen = !is_fullscreen;
        }

        if (is_fullscreen) {
            glfwGetWindowPos(win, &windowed_xpos, &windowed_ypos);
            glfwGetWindowSize(win, &windowed_width, &windowed_height);

            GLFWmonitor* monitor = glfwGetPrimaryMonitor();
            const GLFWvidmode* mode = glfwGetVideoMode(monitor);

            glfwSetWindowMonitor(win, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
        } else {
            glfwSetWindowMonitor(win, nullptr, windowed_xpos, windowed_ypos, windowed_width, windowed_height, 0);
        }
    }
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
    glfwWindowHint(GLFW_SAMPLES, 4);
    static GLFWwindow* window = nullptr;
    glfwSetErrorCallback(error_callback);

    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(windowed_width, windowed_height, "Image Viewer (OpenGL Low-Level)", nullptr, nullptr);
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


    while (!glfwWindowShouldClose(window)) {
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0.1, 0.1, 0.14, 1.0);

        const float texture_aspect = static_cast<float>(text_width) / static_cast<float>(text_height);

        glm::mat4 model(1);

        model = glm::rotate(model, glm::radians(90.f), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, glm::vec3(texture_aspect, 1.0f, 1.0f));

        //model = glm::scale(model, glm::vec3(.8f));


        const float aspect = static_cast<float>(width) / static_cast<float>(height);

        glm::mat4x4 projection;
        if (aspect >= 1.0f) {
            projection = glm::ortho(-aspect, aspect, -1.0f, 1.0f);
        } else {
            projection = glm::ortho(-1.0f, 1.0f, -1.0f / aspect, 1.0f / aspect);
        }

        glm::mat4x4 view(1.0f);

        view = glm::translate(view, glm::vec3(camera_position, 0.0f));
        glm::mat4 mvp = projection * view * model;

        glUniformMatrix4fv(mvp_location, 1, GL_FALSE, glm::value_ptr(mvp));

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glDeleteTextures(1, &texture);
    glDeleteProgram(program);
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &ebo);

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
