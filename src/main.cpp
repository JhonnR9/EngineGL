
#include "app.h"
#include "my_scene.h"


int main() {
    App &app = App::getInstance();
    app.init();
    app.setScene(std::make_unique<MyScene>(app));
    app.game_loop();

    return 0;
}
