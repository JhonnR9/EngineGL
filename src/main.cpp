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
    {{0.f, 0.f}, {0.0f, 0.0f}},
    {{1.f, 0.f}, {1.0f, 0.0f}},
    {{1.f, 1.f}, {1.0f, 1.0f}},
    {{0.f, 1.f}, {0.0f, 1.0f}}
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

    material.set_texture(std::move(std::make_unique<Texture>(RESOURCE_PATH"/texture.jpg")));

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
    glfwSwapInterval(0);

    while (!glfwWindowShouldClose(app.get_window())) {
        material.use();

        int width, height;
        glfwGetFramebufferSize(app.get_window(), &width, &height);

        // mundo virtual fixo
        const float virtualWidth = 840.0f;
        const float virtualHeight = 480.0f;
        float windowAspect = float(width) / float(height);
        float worldAspect = virtualWidth / virtualHeight;

        int viewportX = 0, viewportY = 0;
        int viewportWidth = width, viewportHeight = height;

        if (windowAspect > worldAspect) {

            viewportWidth = int(height * worldAspect);
            viewportX = (width - viewportWidth) / 2;
        } else {
            viewportHeight = int(width / worldAspect);
            viewportY = (height - viewportHeight) / 2;
        }

        glViewport(viewportX, viewportY, viewportWidth, viewportHeight);

        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0.1f, 0.1f, 0.14f, 1.0f);

        glm::mat4 projection = glm::ortho(0.0f, virtualWidth, virtualHeight, 0.0f, -1.0f, 1.0f);

        float scale = 0.2f;
        float texture_w = float(material.get_texture().get_width());
        float texture_h = float(material.get_texture().get_height());

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(300, 100, 0.0f));
        model = glm::rotate(model, glm::radians(90.f), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, glm::vec3(texture_w * scale, texture_h * scale, 1.0f));

        glm::mat4 mvp = projection * model;
        glUniformMatrix4fv(mvp_location, 1, GL_FALSE, glm::value_ptr(mvp));

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
