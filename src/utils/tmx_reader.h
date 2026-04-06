#ifndef TMX_READER_H
#define TMX_READER_H

#include "utils/asset.h"
#include "utils/assets_manager.h"
#include "tinyxml2.h"
#include "tsx_reader.h"
#include <memory>
#include <string>
#include <vector>

class TMXReader : public Asset {
    std::shared_ptr<tinyxml2::XMLDocument> doc;
    std::string basePath;
    AssetsManager* assets_manager{nullptr};

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
    bool parse_tmx();

public:
    TMXReader(const std::vector<char>& raw_data, const std::string& path, AssetsManager* am);

    MapData getMapData() const { return mapData; }
    const std::vector<TSXReader::TilesetData>& getTilesets() const { return tilesets; }
};

#endif