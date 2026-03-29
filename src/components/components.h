#ifndef COMPONENTS_H
#define COMPONENTS_H
#include <string>
#include <unordered_set>
#include "utils/vector2.h"
#include "entt/entt.hpp"
#include "graphics/font.h"
#include "utils/tmx_reader.h"

struct Color {
    float r{1.0f}, g{1.0f}, b{1.0f}, a{1.0f};
};

struct Rect {
    float x{0}, y{0}, width{0.f}, height{0.f};
};

struct Ellipse {
    float cx{0}, cy{0}, rx{0}, ry{0};
};

struct Line {
    Vector2 start;
    Vector2 end;
    float thickness;
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
private:
    std::shared_ptr<TMXReader> map{nullptr};
    int layerIndex{0};

public:
    TileMapLayer(std::shared_ptr<TMXReader> m = nullptr, int layer = 0)
        : map(m), layerIndex(layer) {}

    std::shared_ptr<TMXReader> getMap() const { return map; }
    int getLayerIndex() const { return layerIndex; }

    void setMap(std::shared_ptr<TMXReader> m) { map = m; }
    void setLayerIndex(int layer) {
        if (map && layer >= 0 && layer < static_cast<int>(map->getMapData().layerData.size()))
            layerIndex = layer;
        else
            layerIndex = 0;
    }

    bool isValid() const { return map != nullptr; }
};


#endif //COMPONENTS_H
