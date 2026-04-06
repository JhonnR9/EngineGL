//
// Created by jhone on 22/03/2026.
//

#ifndef RENDER_SYSTEM_H
#define RENDER_SYSTEM_H


#include "system.h"


#include <unordered_map>
#include <memory>

#include "graphics/orthographic_camera.h"
#include "utils/assets_manager.h"

enum class TextureFilter;
class Texture2D;
class Renderer2D;

class RenderSystem : public System {
    struct TileChunk {
        int start_x, start_y;
        Rect bounds;
        std::vector<InstanceData> instances;
    };

    struct Data {
        std::unique_ptr<Renderer2D> renderer{nullptr};
        OrthographicCamera *camera{nullptr};
        std::unique_ptr<OrthographicCamera> ui_camera{nullptr};
        std::unique_ptr<Renderer2D> ui_renderer{nullptr};
        Size ui_viewport_size{1366, 768};
    };


    Data data{};

    AssetsManager *assets_manager{nullptr};
    std::unordered_map<std::string, std::unique_ptr<Texture2D> > texture_cache;

    Texture2D *get_texture(const std::string &path, TextureFilter filter);

    void render_sprite();

    void render_texts();

    void render_shapes();

    void render_tiles();

    void rebuild_tilemap_cache(TileMapLayer &layer, const Transform &transform);

    static bool rects_intersect(const Rect &a, const Rect &b) {
        return !(b.x > a.x + a.width ||
                 b.x + b.width < a.x ||
                 b.y > a.y + a.height ||
                 b.y + b.height < a.y);
    }

public:
    RenderSystem(entt::registry &registry);

    void set_ui_view_size(Size ui_view_size);

    void run(float dt) override;
};


#endif //RENDER_SYSTEM_H
