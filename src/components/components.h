#ifndef COMPONENTS_H
#define COMPONENTS_H
#include <string>
#include <unordered_set>
#include "utils/vector2.h"
#include "entt/entt.hpp"
#include "graphics/font.h"
#include "graphics/sprite_batch.h"
#include "graphics/render_types.h"

struct Size {
    int width{0};
    int height{0};
};

struct SizeF {
    float width{0.0f};
    float height{0.0f};
};



struct ZIndex {
    float value = 0.0f;
};

struct Transform {
    Vector2 position = Vector2(0, 0);
    Vector2 scale = Vector2(1.0f, 1.0f);
    float rotation = 0.0f;
};

struct Sprite {
    std::string texture_path;
    Color color = {1.f, 1.f, 1.f, 1.0f};
    bool flipped_x = false;
    bool flipped_y = false;
    Rect src_rect = Rect();
    TextureFilter filter{TextureFilter::Linear};
};


struct Label {
    enum class TextAlign {
        Left,
        Center,
        Right
    };

    std::string text;
    Color color = {1.f, 1.f, 1.f, 1.f};
    std::shared_ptr<Font> font = nullptr;
    float font_size = 16; //px
    TextAlign align = TextAlign::Left;
};

struct BoxCollider2D {
    float width = 0;
    float height = 0;
    bool is_colliding = false;
    bool is_trigger = false;
    bool is_static = false;
    bool sync_size_with_sprite = true;
    std::unordered_set<entt::entity> colliding_entities;

    BoxCollider2D() = default;

    BoxCollider2D(float width, float height, bool is_colliding, bool is_trigger, bool is_static,
                  bool sync_size_with_sprite)
        : width(width), height(height), is_colliding(is_colliding),
          is_trigger(is_trigger), is_static(is_static), sync_size_with_sprite(sync_size_with_sprite) {
    }
};

struct TileMapLayer {
    struct TileChunk {
        Rect bounds;
        std::vector<InstanceData> instances;
    };

    std::string map_path;
    std::string atlas_path;
    int layer_index = 0;
    int tile_width = 0;
    int tile_height = 0;

    bool dirty = true;
    std::vector<TileChunk> chunks;

    TileMapLayer() = default;

    bool is_valid() const { return !map_path.empty(); }
};

#endif //COMPONENTS_H
