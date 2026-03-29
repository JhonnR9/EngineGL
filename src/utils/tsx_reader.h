//
// Created by jhone on 29/03/2026.
//

#ifndef TSX_READER_H
#define TSX_READER_H

#include "tinyxml2/tinyxml2.h"
#include <memory>
#include <string>

class TSXReader {
    std::shared_ptr<tinyxml2::XMLDocument> doc;

    bool load_tsx(const char *path);
    bool parse_tsx();

public:
    struct TilesetData {
        std::string name;
        int tileWidth;
        int tileHeight;
        std::string imageSource;
        int imageWidth;
        int imageHeight;
    };

private:
    TilesetData tilesetData;

public:
    TSXReader(const char *path);
    TilesetData getTilesetData() const { return tilesetData; }
};

#endif //TSX_READER_H
