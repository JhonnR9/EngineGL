//
// Created by jhone on 22/03/2026.
//

#ifndef BASE_SCENE_H
#define BASE_SCENE_H
#include "entt/entt.hpp"
#include "graphics/sprite_batch.h"

class App;

class Scene {
protected:
    App &app;
    entt::registry &registry;
public:
    explicit Scene(App &app, entt::registry &registry);

    virtual ~Scene() = default;

    virtual void init() = 0;

    virtual void update(float delta) = 0;

    virtual void render(SpriteBatch& batch) = 0;
    virtual void on_key_event( int key, int scancode, int action, int mods)=0;
    virtual void on_mouse_button_event(int button, int action, int x, int y)=0;
    virtual void on_mouse_move_event(int x, int y) =0;
    virtual void on_mouse_wheel_event(int delta)=0;
    virtual void on_resize(int width, int height){}

};


#endif //BASE_SCENE_H
