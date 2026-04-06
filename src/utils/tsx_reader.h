#ifndef TSX_READER_H
#define TSX_READER_H

#include "utils/asset.h"
#include "tinyxml2.h"
#include <memory>
#include <string>
#include <vector>

class TSXReader : public Asset {
    std::shared_ptr<tinyxml2::XMLDocument> doc;

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
    bool parse_tsx();

public:
    // Construtor compatível com AssetsManager
    TSXReader(const std::vector<char>& raw_data);
    
    // Mantido para compatibilidade manual se necessário
    TSXReader(const char *path);

    TilesetData getTilesetData() const { return tilesetData; }
};

#endif