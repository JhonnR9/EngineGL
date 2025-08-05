#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Mesh.h"
#include "Shader.h"
#include "cmake-build-debug/_deps/glfw-src/deps/linmath.h"

static constexpr struct {
    float x, y;
    float r, g, b;
} vertices[3] = {
    {-0.6f, -0.4f, 1.f, 0.f, 0.f},
    {0.6f, -0.4f, 0.f, 1.f, 0.f},
    {0.f, 0.6f, 0.f, 0.f, 1.f}
};


static void error_callback(int error, const char *description) {
    fprintf(stderr, "Error: %s\n", description);
}

static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}


int main() {
    EngineGL::Shader shader(RESOURCE_PATH"/triangle.vert", RESOURCE_PATH"/triangle.frag");
    EngineGL::Mesh mesh;

    GLFWwindow *window;
    GLuint vao, vertex_buffer, vertex_shader, fragment_shader, program;
    GLint mvp_location;

    glfwSetErrorCallback(error_callback);

    if (!glfwInit())
        exit(EXIT_FAILURE);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Para Mac

    window = glfwCreateWindow(640, 480, "Simple example", NULL, NULL);
    if (!window) {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwSetKeyCallback(window, key_callback);
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        fprintf(stderr, "Failed to initialize GLAD\n");
        return -1;
    }

    glfwSwapInterval(1);


    mesh.create_buffer(GL_ARRAY_BUFFER);
    EngineGL::Mesh::define_buffer_data(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);


    shader.compile_shaders();


    mvp_location = glGetUniformLocation(shader.get_program_id(), "MVP");

    EngineGL::Mesh::set_vertex_attrib_pointer(
        0, 2, GL_FLOAT, GL_FALSE,
        sizeof(vertices[0]), static_cast<void *>(nullptr)
    );
    EngineGL::Mesh::set_vertex_attrib_pointer(
    1, 3, GL_FLOAT, GL_FALSE,
    sizeof(vertices[0]), reinterpret_cast<void *>(sizeof(float) * 2)
    );

    while (!glfwWindowShouldClose(window)) {
        int width, height;
        mat4x4 m, p, mvp;

        glfwGetFramebufferSize(window, &width, &height);
        const float ratio = width / (float) height;

        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);

        mat4x4_identity(m);
        mat4x4_rotate_Z(m, m, (float) glfwGetTime());
        mat4x4_ortho(p, -ratio, ratio, -1.f, 1.f, 1.f, -1.f);
        mat4x4_mul(mvp, p, m);

        shader.bind();
        glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (const GLfloat *) mvp);

        mesh.bind();

        glDrawArrays(GL_TRIANGLES, 0, 3);
        EngineGL::Mesh::unbind();
        EngineGL::Shader::unbind();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }


    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
