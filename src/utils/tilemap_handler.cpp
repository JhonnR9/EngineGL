//
// Created by jhone on 06/04/2026.
//

#include "tilemap_handler.h"

#include <iostream>
#include "utils/assets_manager.h"
#include "assets_manager.h"
#include "resource_types/tilemap.h"

TilemapHandler::TilemapHandler(AssetsManager *manager) {
    this->assets_manager = manager;
}

void TilemapHandler::add_tilemap_layer(TilemapLayerComponent &layer, Transform &transform) {
    LayerData ld{layer, transform};
    layers_entity_cache.emplace_back(ld);
}

void TilemapHandler::process_layers() {
    if (!assets_manager) {
        std::cerr << "[TilemapHandler] Error, no assets manager provided!" << std::endl;
        return;
    }
    for (const auto &layer_entity: layers_entity_cache) {
        if (layer_entity.layer.tmx.status == File::FileStatus::Pending) {
             if(!assets_manager->is_file_preload(layer_entity.layer.tmx.path.c_str())){

            }
        }
    }
}

void TilemapHandler::reset() {
    const size_t size = layers_entity_cache.size();
    layers_entity_cache.clear();
    layers_entity_cache.reserve(size);
}
