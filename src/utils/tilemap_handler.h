//
// Created by jhone on 06/04/2026.
//

#ifndef TILEMAP_HANDLER_H
#define TILEMAP_HANDLER_H
#include "components/components.h"
#include <unordered_map>

class AssetsManager;

class TilemapHandler {
    AssetsManager *assets_manager{nullptr};
    struct LayerData {
        TilemapLayerComponent& layer;
        Transform& transform;
        bool dirty{true};
    };

    std::vector<LayerData> layers_entity_cache;
    std::unordered_map<int, >

public:
    TilemapHandler(AssetsManager* manager);

    void add_tilemap_layer(TilemapLayerComponent& layer, Transform& transform);
    void process_layers();

    void reset();
};



#endif //TILEMAP_HANDLER_H
