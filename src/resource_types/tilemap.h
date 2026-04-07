//
// Created by jhone on 06/04/2026.
//

#ifndef TILEMAP_H
#define TILEMAP_H
#include "utils/asset.h"
#include <string>
#include <vector>

struct TilemapLayer {
    int id{-1};
    std::string name;
    std::string csv_data;
};

class Tilemap final : public Asset {
    int map_width{0};
    int map_height{0};
    std::vector<TilemapLayer> layers;
    std::vector<std::string> used_tilesets;

    void parse(const std::vector<char> &raw_data);

public:
    explicit Tilemap(const std::vector<char> &raw_data);

    int get_map_width() const {
        return map_width;
    }

    int get_map_height() const {
        return map_height;
    }

    const std::vector<TilemapLayer>& get_layers() const {
        return layers;
    }

    const std::vector<std::string>& get_used_tilesets() const {
        return used_tilesets;
    }
};


#endif //TILEMAP_H
