//
// Created by jhone on 22/03/2026.
//

#include "render_system.h"
#include "graphics/sprite_batch.h"
#include "graphics/texture_2d.h"

#include "components/components.h"

RenderSystem::RenderSystem(entt::registry &registry, SpriteBatch &batch)
    : System(registry), batch(batch) {
}

Texture2D *RenderSystem::get_texture(const std::string &path) {
    if (const auto it = texture_cache.find(path); it != texture_cache.end()) {
        return it->second.get();
    }
    auto texture = std::make_unique<Texture2D>(path.c_str());
    auto* ptr = texture.get();

    texture_cache.emplace(path, std::move(texture));
    return ptr;

}

void RenderSystem::run(float dt) {
    batch.begin();

    auto view = registry.view<Transform, Sprite>();

    for (auto entity: view) {
        auto &transform = view.get<Transform>(entity);
        auto &sprite = view.get<Sprite>(entity);

        Texture2D *texture = get_texture(sprite.texture_path);

        if (!texture || texture->get_texture() == 0)
            continue;

        auto z = registry.try_get<ZIndex>(entity);
        float z_index = z ? z->value : 0.0f;

        batch.draw_texture(
            texture,
            transform.position,
            transform.scale,
            transform.rotation,
            Vector2(texture->get_width() / 2.f, texture->get_height() / 2.f),
            sprite.color,
            sprite.src_rect,
            sprite.flipped_x,
            sprite.flipped_y,
            z_index
        );
    }

    batch.end();
}

