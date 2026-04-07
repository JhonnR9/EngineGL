//
// Created by jhone on 06/04/2026.
//

#ifndef TILESET_H
#define TILESET_H

#include "utils/asset.h"
#include <string>
#include <vector>

class Tileset final : public Asset {
    std::string name;
    int tile_width{0};
    int tile_height{0};
    std::string image_source;
    int image_width{0};
    int image_height{0};

    void parse(const std::vector<char> &raw_data);

public:
    Tileset(const std::vector<char> &raw_data);

};


#endif //TILESET_H
