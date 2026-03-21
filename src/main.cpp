#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include <cmath> // Para std::sin

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "app.h"
#include "graphics/Material.h"
#include "graphics/Shader.h"
#include "graphics/Texture.h"

struct Vertex {
    glm::vec2 pos;
    glm::vec2 texCoord;
};

Vertex vertices[4] = {
    {{-0.5f, -0.5f}, {0.0f, 0.0f}},
    {{ 0.5f, -0.5f}, {1.0f, 0.0f}},
    {{ 0.5f,  0.5f}, {1.0f, 1.0f}},
    {{-0.5f,  0.5f}, {0.0f, 1.0f}}
};

static constexpr GLuint indices[6] = {
    0, 1, 2,
    2, 3, 0
};

int main() {
    App &app = App::getInstance();
    app.init();

    constexpr int num_instances = 750;

    std::default_random_engine generator;
    std::uniform_real_distribution randCoordX(0.0f, 1366.0f);
    std::uniform_real_distribution randCoordY(0.0f, 768.0f);
    std::uniform_real_distribution randScale(0.05f, 0.3f);
    std::uniform_real_distribution randColor(0.0f, 1.0f);

    struct InstanceData {
        glm::vec2 pos;
        float scale;
        float floatOffset;
    };

    std::vector<InstanceData> instanceTransforms(num_instances);
    std::vector<glm::vec3> instanceColors(num_instances);

    for (int i = 0; i < num_instances; ++i) {
        instanceTransforms[i] = {
            glm::vec2(randCoordX(generator), randCoordY(generator)),
            randScale(generator),
            (float)i
        };
        instanceColors[i] = glm::vec3(randColor(generator), randColor(generator), randColor(generator));
    }

    Material material;
    material.set_shader(std::make_unique<Shader>(
        SourcePath(RESOURCE_PATH"/default_shader.vert", RESOURCE_PATH"/default_shader.frag")
    ));
    material.set_texture(std::move(std::make_unique<Texture>(RESOURCE_PATH"/hutao.png")));

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoord));

    GLuint ebo;
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    GLuint colorVBO;
    glGenBuffers(1, &colorVBO);
    glBindBuffer(GL_ARRAY_BUFFER, colorVBO);
    glBufferData(GL_ARRAY_BUFFER, instanceColors.size() * sizeof(glm::vec3), instanceColors.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
    glVertexAttribDivisor(2, 1);

    std::vector<glm::mat4> mvps(num_instances);
    GLuint instanceVBO;
    glGenBuffers(1, &instanceVBO);
    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    glBufferData(GL_ARRAY_BUFFER, num_instances * sizeof(glm::mat4), nullptr, GL_DYNAMIC_DRAW);

    for (int i = 0; i < 4; i++) {
        glEnableVertexAttribArray(3 + i);
        glVertexAttribPointer(3 + i, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4) * i));
        glVertexAttribDivisor(3 + i, 1);
    }

    glfwSwapInterval(0);

    while (!glfwWindowShouldClose(app.get_window())) {
        int width, height;
        glfwGetFramebufferSize(app.get_window(), &width, &height);

        const float virtual_width = 1366.0f;
        const float virtual_height = 768.0f;

        float windowAspect = float(width) / float(height);
        float worldAspect = virtual_width / virtual_height;

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

        glm::mat4 projection = glm::ortho(0.0f, virtual_width, virtual_height, 0.0f, -1.0f, 1.0f);

        float texW = float(material.get_texture().get_width());
        float texH = float(material.get_texture().get_height());
        float time = (float)glfwGetTime();

        for (int i = 0; i < num_instances; ++i) {

            float offsetY = std::sin(time * 2.0f + instanceTransforms[i].floatOffset) * 200.0f;

            glm::mat4 model = glm::mat4(1.0f);

            model = glm::translate(model, glm::vec3(instanceTransforms[i].pos.x ,
                                                    instanceTransforms[i].pos.y+ offsetY ,
                                                    0.0f));
            model = glm::rotate(model, time + i, glm::vec3(0.0f, 0.0f, 1.0f));

            model = glm::scale(model, glm::vec3(texW * 0.25f,
                                                texH * 0.25f, 1.0f));

            mvps[i] = projection * model;
        }

        glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, mvps.size() * sizeof(glm::mat4), mvps.data());

        material.use();
        glBindVertexArray(vao);
        glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr, num_instances);

        glfwSwapBuffers(app.get_window());
        glfwPollEvents();
    }

    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &colorVBO);
    glDeleteBuffers(1, &instanceVBO);
    glDeleteBuffers(1, &ebo);
    glDeleteVertexArrays(1, &vao);

    glfwDestroyWindow(app.get_window());
    glfwTerminate();
    return 0;
}