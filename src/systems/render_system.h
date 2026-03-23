//
// Created by jhone on 22/03/2026.
//

#ifndef RENDER_SYSTEM_H
#define RENDER_SYSTEM_H



#include "system.h"


#include <unordered_map>
#include <memory>

class Texture2D;
class SpriteBatch;

class RenderSystem : public System {
private:
    SpriteBatch& batch;

    std::unordered_map<std::string, std::unique_ptr<Texture2D>> texture_cache;

    Texture2D* get_texture(const std::string& path);

public:
    RenderSystem(entt::registry& registry, SpriteBatch& batch);

    void run(float dt) override;
};



#endif //RENDER_SYSTEM_H
