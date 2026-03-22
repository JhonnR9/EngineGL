#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <cstdlib>
#include <ctime>
#include "app.h"
#include "vector2.h"
#include "graphics/SpriteBatch.h"
#include "graphics/texture_2d.h"

int main() {
    App &app = App::getInstance();
    app.init();
    glfwSwapInterval(0);
    SpriteBatch batch(app.get_windowed_width(), app.get_windowed_height());

    Texture2D texture1(RESOURCE_PATH"/hutao.png");
    Texture2D texture2(RESOURCE_PATH"/texture.jpg");


    const int num_draws = 500;
    const unsigned int seed = static_cast<unsigned int>(time(nullptr));
    float lastTime = glfwGetTime();

    while (!glfwWindowShouldClose(app.get_window())) {
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        float currentTime = glfwGetTime();
        float deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        batch.begin();

        srand(seed);

        float rotation = currentTime * (0.5f + static_cast<float>(rand()) / RAND_MAX);


        for (int i = 0; i < num_draws; ++i) {
            Texture2D* textureToUse = &texture1;

            float x = static_cast<float>(rand() % app.get_windowed_width());
            float y = static_cast<float>(rand() % app.get_windowed_height());

            Color c;
            c.r = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
            c.g = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
            c.b = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
            c.a = 1.0f;

            batch.draw_texture(
                textureToUse,
                Vector2(x, y),
                Vector2(1.0f, 1.0f),
                rotation,
                Vector2(textureToUse->get_width() / 2.f, textureToUse->get_height() / 2.f),
                c,
                Rect()
            );
        }

        batch.end();

        glfwSwapBuffers(app.get_window());
        glfwPollEvents();
    }

    glfwDestroyWindow(app.get_window());
    glfwTerminate();
    return 0;
}