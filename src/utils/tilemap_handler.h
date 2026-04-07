//
// Created by jhone on 06/04/2026.
//

#ifndef TILEMAP_HANDLER_H
#define TILEMAP_HANDLER_H
#include "components/components.h"


class TilemapHandler {
    struct LayerData {
        TilemapLayerComponent& layer;
        Transform& transform;
    };

    std::vector<LayerData> layers_cache;

public:
    TilemapHandler()=default;

    void add_tilemap_layer(TilemapLayerComponent& layer, Transform& transform);

    void reset();
};



#endif //TILEMAP_HANDLER_H
