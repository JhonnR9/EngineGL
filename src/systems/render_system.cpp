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

    auto texture_view = registry.view<Transform, Sprite>();

    for (auto entity: texture_view) {
        auto &transform = texture_view.get<Transform>(entity);
        auto &sprite = texture_view.get<Sprite>(entity);

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

    auto rect_view = registry.view<Transform, Rect, Color>();
    for (auto entity: rect_view) {
        auto &transform = rect_view.get<Transform>(entity);
        auto &rect = rect_view.get<Rect>(entity);
        auto &color = rect_view.get<Color>(entity);

        auto z = registry.try_get<ZIndex>(entity);
        float z_index = z ? z->value : 0.0f;

        Rect world_rect;
        world_rect.x = transform.position.x;
        world_rect.y = transform.position.y;
        world_rect.width = rect.width * transform.scale.x;
        world_rect.height = rect.height * transform.scale.y;

        batch.draw_rect(
            world_rect,
            color,
            Vector2(0.5f, 0.5f),
            transform.rotation,
            z_index
        );
    }

    auto ellipse_view = registry.view<Transform, Ellipse, Color>();
    for (auto entity: ellipse_view) {
        auto &transform = ellipse_view.get<Transform>(entity);
        auto &ellipse = ellipse_view.get<Ellipse>(entity);
        auto &color = ellipse_view.get<Color>(entity);

        auto z = registry.try_get<ZIndex>(entity);
        float z_index = z ? z->value : 0.0f;

        Ellipse world_ellipse;
        world_ellipse.cx = transform.position.x;
        world_ellipse.cy = transform.position.y;
        world_ellipse.rx = ellipse.rx * transform.scale.x;
        world_ellipse.ry = ellipse.ry * transform.scale.y;

        batch.draw_ellipse(
            world_ellipse,
            color,
            z_index
        );
    }

    auto line_view = registry.view<Transform, Line, Color>();
    for (auto entity: line_view) {
        auto &transform = line_view.get<Transform>(entity);
        auto &line = line_view.get<Line>(entity);
        auto &color = line_view.get<Color>(entity);

        auto z = registry.try_get<ZIndex>(entity);
        float z_index = z ? z->value : 0.0f;

        Vector2 start = {
            line.start.x * transform.scale.x + transform.position.x,
            line.start.y * transform.scale.y + transform.position.y
        };

        Vector2 end = {
            line.end.x * transform.scale.x + transform.position.x,
            line.end.y * transform.scale.y + transform.position.y
        };

        batch.draw_line(
            start,
            end,
            line.thickness * transform.scale.y,
            color,
            z_index
        );
    }

    auto label_view = registry.view<Transform, Label>();

    for (auto entity : label_view) {
        auto &transform = label_view.get<Transform>(entity);
        auto &label = label_view.get<Label>(entity);

        if (!label.font) {
            continue;
        }

        auto z = registry.try_get<ZIndex>(entity);
        float z_index = z ? z->value : 0.0f;

        float scale = label.font_size / (float)label.font->get_font_size();

        float pen_x = 0.0f;
        float text_width = 0.0f;

        for (char c : label.text) {
            const Character &ch = label.font->characters.at(c);

            float glyph_end = pen_x + ch.bearing.x * scale + ch.size.x * scale;
            text_width = std::max(text_width, glyph_end);

            pen_x += ch.advance * scale;
        }

        float x = transform.position.x;

        switch (label.align) {
            case Label::TextAlign::Center:
                x -= text_width * 0.5f;
                break;
            case Label::TextAlign::Right:
                x -= text_width;
                break;
            case Label::TextAlign::Left:
            default:
                break;
        }

        float y = transform.position.y;

        batch.draw_text(
            *label.font,
            label.text,
            Vector2{x, y},
            scale,
            label.color,
            z_index
        );
    }

    batch.end();
}

