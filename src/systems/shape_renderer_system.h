//
// Created by jhone on 25/03/2026.
//

#ifndef SHAPE_RENDERER_SYSTEM_H
#define SHAPE_RENDERER_SYSTEM_H

#include <entt/entt.hpp>
#include "system.h"
#include "graphics/shape_renderer.h"

class ShapeRendererSystem : public System {
public:
    ShapeRendererSystem(entt::registry& registry);

    void run(float dt) override;

private:
    ShapeRenderer* shape_renderer{nullptr};
};




#endif //SHAPE_RENDERER_SYSTEM_H
