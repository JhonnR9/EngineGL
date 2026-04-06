//
// Created by jhone on 22/03/2026.
//

#ifndef BASE_SCENE_H
#define BASE_SCENE_H
#include "entt/entt.hpp"
#include "graphics/sprite_batch.h"
#include "events/event.h"

class AssetsManager;
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

    virtual void on_event(const Event &e)=0;
    virtual void load_assets(AssetsManager* manager)=0;

};


#endif //BASE_SCENE_H
