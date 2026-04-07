//
// Created by jhone on 06/04/2026.
//

#include "tilemap_handler.h"

void TilemapHandler::add_tilemap_layer(TilemapLayerComponent &layer, Transform &transform) {
    LayerData ld{layer, transform};
    layers_cache.emplace_back(ld);

}

void TilemapHandler::reset() {
    const size_t size = layers_cache.size();
    layers_cache.clear();
    layers_cache.reserve(size);
}
