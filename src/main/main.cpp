#include "app.h"
#include "scenes/my_scene.h"

int main() {
    const auto app = std::make_unique<App>();
    app->init();

    auto &registry = app->get_registry();

    app->setScene(std::make_unique<MyScene>(*app, registry));
    app->game_loop();

    return 0;
}
