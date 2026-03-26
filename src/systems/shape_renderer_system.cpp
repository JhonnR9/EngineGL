//
// Created by jhone on 25/03/2026.
//

#include "shape_renderer_system.h"

#include "components/components.h"

ShapeRendererSystem::ShapeRendererSystem(entt::registry &registry)
    : System(registry) {

    shape_renderer = registry.ctx().get<ShapeRenderer *>();
}

void ShapeRendererSystem::run(float dt) {
    if (!shape_renderer)return;
    shape_renderer->begin();

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

        shape_renderer->draw_rect(
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

        shape_renderer->draw_eclipse(
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

        shape_renderer->draw_line(
            start,
            end,
            line.thickness * transform.scale.y,
            color,
            z_index
        );
    }

    shape_renderer->end();
}
