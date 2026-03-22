#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <cstdlib>
#include <ctime>
#include "app.h"
#include "vector2.h"
#include "graphics/renderer.h"
#include "graphics/texture_2d.h"

int main() {
    App &app = App::getInstance();
    app.init();
    glfwSwapInterval(0);
    Renderer renderer(app.get_windowed_width(), app.get_windowed_height());

    Texture2D texture1(RESOURCE_PATH"/texture.jpg");


    const int num_draws = 1000;
    unsigned int seed = static_cast<unsigned int>(time(NULL));

    while (!glfwWindowShouldClose(app.get_window())) {
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        renderer.begin();

        srand(seed);

        for (int i = 0; i < num_draws; ++i) {


            float x = static_cast<float>(rand() % app.get_windowed_width());
            float y = static_cast<float>(rand() % app.get_windowed_height());

            Color c;
            c.r = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
            c.g = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
            c.b = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
            c.a = 1.0f;

            renderer.draw_texture(
                &texture1,
                Vector2(x, y),
                Vector2(0.25f, 0.25f),
                0.0f,
                Vector2(texture1.get_width() / 2.f, texture1.get_height() / 2.f),
                c
            );
        }

        renderer.end();

        glfwSwapBuffers(app.get_window());
        glfwPollEvents();
    }

    glfwDestroyWindow(app.get_window());
    glfwTerminate();
    return 0;
}