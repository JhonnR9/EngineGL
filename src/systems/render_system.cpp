//
// Created by jhone on 22/03/2026.
//

#include "render_system.h"

#include <filesystem>
#include <SFML/System/Exception.hpp>
#include "graphics/sprite_batch.h"
#include "graphics/texture_2d.h"
#include "components/components.h"
#include "utils/tmx_reader.h"

RenderSystem::RenderSystem(entt::registry &registry)
    : System(registry) {
    if (registry.ctx().contains<OrthographicCamera *>()) {
        data.camera = registry.ctx().get<OrthographicCamera *>();

        this->data.renderer = std::make_unique<Renderer2D>(data.camera);

        data.ui_camera = std::make_unique<OrthographicCamera>(data.ui_viewport_size);
        data.ui_renderer = std::make_unique<Renderer2D>(data.ui_camera.get());
    } else {
        throw std::runtime_error(
            "No valid camera found in the application context. "
            "Rendering system cannot function without a valid camera."
        );
    }


    if (registry.ctx().contains<AssetsManager *>()) {
        assets_manager = registry.ctx().get<AssetsManager *>();
    } else {
        throw std::runtime_error(
            "[RenderSystem] No valid [AssetManager] found in the application context. "
            "Rendering system cannot function without a valid [AssetManager]."
        );
    }
}

void RenderSystem::set_ui_view_size(Size ui_view_size) {
    if (ui_view_size.width <= 0 || ui_view_size.height <= 0) {
        std::cerr << "Invalid view size using default." << std::endl;
        return;
    }
    this->data.ui_viewport_size = ui_view_size;
}

void RenderSystem::run(float dt) {
    data.renderer->begin();

    glClearColor(0.043f, 0.067f, 0.090f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    render_tiles();
    render_sprite();
    render_shapes();


    data.renderer->end();
    data.ui_renderer->begin();
    render_texts();
    data.ui_renderer->end();
}

Texture2D *RenderSystem::get_texture(const std::string &path, TextureFilter filter) {
    if (const auto it = texture_cache.find(path); it != texture_cache.end()) {
        return it->second.get();
    }
    auto texture = std::make_unique<Texture2D>(path.c_str(), filter);
    auto *ptr = texture.get();

    texture_cache.emplace(path, std::move(texture));
    return ptr;
}

void RenderSystem::render_sprite() {
    auto texture_view = registry.view<Transform, Sprite>();

    for (auto entity: texture_view) {
        auto &transform = texture_view.get<Transform>(entity);
        auto &sprite = texture_view.get<Sprite>(entity);

        auto texture = assets_manager->get_asset<Texture2D>(
            sprite.texture_path,
            sprite.filter
        );

        if (!texture) continue;


        auto z = registry.try_get<ZIndex>(entity);
        float z_index = z ? z->value : 0.0f;

        data.renderer->draw_texture(
            texture.get(),
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
}

void RenderSystem::render_texts() {
    auto label_view = registry.view<Transform, Label>();

    for (auto entity: label_view) {
        auto &transform = label_view.get<Transform>(entity);
        auto &label = label_view.get<Label>(entity);

        if (!label.font) {
            continue;
        }

        auto z = registry.try_get<ZIndex>(entity);
        float z_index = z ? z->value : 0.0f;

        float scale = label.font_size / static_cast<float>(label.font->get_font_size());

        float pen_x = 0.0f;
        float text_width = 0.0f;

        for (char c: label.text) {
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

        data.ui_renderer->draw_text(
            *label.font,
            label.text,
            Vector2{x, y},
            scale,
            label.color,
            z_index
        );
    }
}

void RenderSystem::render_shapes() {
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

        data.renderer->draw_rect(
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

        data.renderer->draw_ellipse(
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

        data.renderer->draw_line(
            start,
            end,
            line.thickness * transform.scale.y,
            color,
            z_index
        );
    }
}

void RenderSystem::render_tiles() {
    auto view = registry.view<TileMapLayer, Transform>();
    Rect camera_rect = data.camera->get_view_rect();

    for (auto entity : view) {
        auto& layer = view.get<TileMapLayer>(entity);
        auto& transform = view.get<Transform>(entity);

        Vector2 tile_size = Vector2(layer.tile_width, layer.tile_height);

        float margin_x = tile_size.x * 2.0f;
        float margin_y = tile_size.y * 2.0f;

        camera_rect.x      -= margin_x;
        camera_rect.y      -= margin_y;
        camera_rect.width  += margin_x * 2;
        camera_rect.height += margin_y * 2;


        if (!layer.is_valid()) continue;

        if (layer.dirty) {
            rebuild_tilemap_cache(layer, transform);

            if (layer.dirty) continue;
        }

        auto texture = assets_manager->get_asset<Texture2D>(layer.atlas_path, TextureFilter::Nearest);
        if (!texture) continue;

        for (auto& chunk : layer.chunks) {

            if (!rects_intersect(chunk.bounds, camera_rect)) continue;

            std::vector<InstanceData> visible_tiles;
            for (auto& inst : chunk.instances) {
                Rect inst_rect = { inst.position.x, inst.position.y, inst.scale.x, inst.scale.y };
                if (rects_intersect(inst_rect, camera_rect)) {
                    visible_tiles.push_back(inst);
                }
            }

            if (!visible_tiles.empty()) {
                data.renderer->draw_instances(texture.get(), visible_tiles);
            }
        }
    }
}

void RenderSystem::rebuild_tilemap_cache(TileMapLayer &layer, const Transform &transform) {
    if (layer.map_path.empty()) return;

    if (!assets_manager->is_file_preload(layer.map_path.c_str())) {
        if (!assets_manager->is_file_preload(layer.map_path.c_str())) {
            assets_manager->queue_file_load(layer.map_path.c_str());
        }
        return;
    }

    auto map = assets_manager->get_asset<TMXReader>(
        layer.map_path,
        layer.map_path,
        assets_manager
    );

    if (!map) return;


    layer.chunks.clear();

    auto mapData = map->getMapData();
    std::string csv = mapData.layerData;
    if (csv.empty()) return;


    std::vector<int> tiles;
    tiles.reserve(mapData.width * mapData.height);
    size_t start = 0;
    while (start < csv.size()) {
        size_t end = csv.find_first_of(",\n", start);
        if (end == std::string::npos) end = csv.size();
        std::string val = csv.substr(start, end - start);
        if (val.find_first_not_of(" \t\n\r") != std::string::npos) {
            tiles.push_back(std::stoi(val));
        }
        start = end + 1;
    }

    const auto &tilesets = map->getTilesets();
    if (tilesets.empty()) return;
    const auto &ts = tilesets[0];

    std::filesystem::path mapPath(layer.map_path);
    layer.atlas_path = (mapPath.parent_path() / ts.imageSource).string();

    if (!assets_manager->is_file_preload(layer.atlas_path.c_str())) {
        assets_manager->queue_file_load(layer.atlas_path.c_str());
    }

    float tex_w = static_cast<float>(ts.imageWidth);
    float tex_h = static_cast<float>(ts.imageHeight);
    if (ts.tileWidth == 0 || ts.tileHeight == 0) return;
    int tilesPerRow = ts.imageWidth / ts.tileWidth;

    const int CHUNK_SIZE = 16;
    std::map<std::pair<int, int>, TileMapLayer::TileChunk> temp_map;

    for (int y = 0; y < mapData.height; ++y) {
        for (int x = 0; x < mapData.width; ++x) {
            int tileIdx = x + y * mapData.width;
            if (tileIdx >= tiles.size()) break;

            int tileId = tiles[tileIdx];
            if (tileId <= 0) continue;

            int cx = x / CHUNK_SIZE;
            int cy = y / CHUNK_SIZE;
            auto& chunk = temp_map[{cx, cy}];

            if (chunk.instances.empty()) {
                chunk.bounds.x = transform.position.x + (cx * CHUNK_SIZE * ts.tileWidth * transform.scale.x);
                chunk.bounds.y = transform.position.y + (cy * CHUNK_SIZE * ts.tileHeight * transform.scale.y);
                chunk.bounds.width = CHUNK_SIZE * ts.tileWidth * transform.scale.x;
                chunk.bounds.height = CHUNK_SIZE * ts.tileHeight * transform.scale.y;
            }

            int localId = tileId - 1;
            int tx = localId % tilesPerRow;
            int ty = localId / tilesPerRow;

            float texelX = 0.5f / tex_w;
            float texelY = 0.5f / tex_h;
            InstanceData inst{};

            inst.region.x = tx * ts.tileWidth / tex_w + texelX;
            inst.region.y = ty * ts.tileHeight / tex_h + texelY;
            inst.region.z = ts.tileWidth / tex_w - 2.0f * texelX;
            inst.region.w = ts.tileHeight / tex_h - 2.0f * texelY;

            inst.position.x = transform.position.x + (x * ts.tileWidth * transform.scale.x);
            inst.position.y = transform.position.y + (y * ts.tileHeight * transform.scale.y);
            inst.scale.x = ts.tileWidth * transform.scale.x;
            inst.scale.y = ts.tileHeight * transform.scale.y;

            inst.color = {1.0f, 1.0f, 1.0f, 1.0f};
            inst.z_index = 0.0f;
            inst.origin = {0.0f, 0.0f};
            inst.shape_type = 0;

            chunk.instances.push_back(inst);
        }
    }

    for (auto& pair : temp_map) {
        layer.chunks.push_back(std::move(pair.second));
    }

    layer.tile_width = ts.tileWidth;
    layer.tile_height = ts.tileHeight;
    layer.dirty = false;
}
