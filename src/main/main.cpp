
#include "app.h"
#include "scenes/my_scene.h"

int main() {
    App &app = App::getInstance();
    app.init();
    entt::registry &registry = app.get_registry();
    app.setScene(std::make_unique<MyScene>(app, registry));
    app.game_loop();
    return 0;
}
