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

#include "app.h"
#include "graphics/Material.h"
#include "graphics/Shader.h"
#include "graphics/Texture.h"

struct Vertex {
    glm::vec2 pos;
    glm::vec2 texCoord;
};

Vertex vertices[4] = {
    {{-1.f, 1.f}, {0.0f, 0.0f,}},
    {{1.f, 1.f}, {1.0f, 0.0f,}},
    {{1.f, -1.f}, {1.f, 1.0f}},
    {{-1.f, -1.f}, {0.0f, 1.0f}}
};

static constexpr GLuint indices[6] = {
    0, 1, 2,
    2, 3, 0
};


int main() {
    App &app = App::getInstance();
    app.init();
    Material material;
    material.set_shader(
        std::make_unique<Shader>(
            SourcePath(RESOURCE_PATH"/default_shader.vert", RESOURCE_PATH"/default_shader.frag")
        )
    );
    const Texture texture(RESOURCE_PATH"/texture.jpg");
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

    const auto shader = Shader(SourcePath(RESOURCE_PATH"/default_shader.vert",RESOURCE_PATH"/default_shader.frag"));
    const GLint mvp_location = material.get_uniform_location("MVP");
    // position x and y  (2 floats)
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), static_cast<void *>(nullptr));

    // texCoords (2 floats)
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          reinterpret_cast<void *>(offsetof(Vertex, texCoord)));
    glEnableVertexAttribArray(1);




    while (!glfwWindowShouldClose(app.get_window())) {
        shader.use();
        int width, height;
        glfwGetFramebufferSize(app.get_window(), &width, &height);
        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0.1, 0.1, 0.14, 1.0);

        const float texture_aspect = static_cast<float>(texture.get_width()) / static_cast<float>(texture.get_height());
        glm::mat4 model(1);

       // model = glm::rotate(model, glm::radians(90.f), glm::vec3(0.0f, 0.0f, 1.0f));
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

        texture.use();

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);


        glfwSwapBuffers(app.get_window());
        glfwPollEvents();
    }
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &ebo);

    glfwDestroyWindow(app.get_window());
    glfwTerminate();
    return 0;
}
