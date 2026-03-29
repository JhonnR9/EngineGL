//
// Created by jhone on 29/03/2026.
//

#ifndef TMX_READER_H
#define TMX_READER_H

#include "tinyxml2/tinyxml2.h"
#include "tsx_reader.h"
#include <memory>
#include <string>
#include <vector>

class TMXReader {
    std::shared_ptr<tinyxml2::XMLDocument> doc;
    std::string basePath;

    bool load_tmx(const char *path);
    bool parse_tmx();

public:
    struct MapData {
        int width;
        int height;
        std::string layerName;
        std::string layerData;
    };

private:
    MapData mapData;
    std::vector<TSXReader::TilesetData> tilesets;

public:
    TMXReader(const char *path);

    MapData getMapData() const { return mapData; }
    const std::vector<TSXReader::TilesetData>& getTilesets() const { return tilesets; }
    std::string getBasePath() const { return basePath; }
};

#endif //TMX_READER_H
