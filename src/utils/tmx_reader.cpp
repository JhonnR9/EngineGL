#include "tmx_reader.h"
#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

TMXReader::TMXReader(const std::vector<char>& raw_data, const std::string& path, AssetsManager* am)
    : assets_manager(am) {
    fs::path tmxpath(path);
    basePath = tmxpath.parent_path().string();

    doc = std::make_shared<tinyxml2::XMLDocument>();
    if (doc->Parse(raw_data.data(), raw_data.size()) == tinyxml2::XML_SUCCESS) {
        parse_tmx();
    }
}

bool TMXReader::parse_tmx() {
    tinyxml2::XMLElement* map = doc->FirstChildElement("map");
    if (!map) return false;

    map->QueryIntAttribute("width", &mapData.width);
    map->QueryIntAttribute("height", &mapData.height);


    if (tinyxml2::XMLElement* layer = map->FirstChildElement("layer")) {
        mapData.layerName = layer->Attribute("name") ? layer->Attribute("name") : "";

        tinyxml2::XMLElement* data = layer->FirstChildElement("data");
        mapData.layerData = (data && data->GetText()) ? data->GetText() : "";
    }

    tilesets.clear();


    for (tinyxml2::XMLElement* ts = map->FirstChildElement("tileset"); ts; ts = ts->NextSiblingElement("tileset")) {
        TSXReader::TilesetData tsData{};
        const char* source = ts->Attribute("source");

        if (source && assets_manager) {
            std::string tsxPath = (fs::path(basePath) / fs::path(source)).string();

            assets_manager->queue_file_load(tsxPath.c_str());

            assets_manager->pool_works();


            auto tsxAsset = assets_manager->get_asset<TSXReader>(tsxPath);

            if (!tsxAsset) {
                std::cerr << "[TMXReader] Falha ao carregar TSX: " << tsxPath << "\n";
                return false;
            }

            tsData = tsxAsset->getTilesetData();


            if (!tsData.imageSource.empty()) {
                std::string imgPath = (fs::path(basePath) / fs::path(tsData.imageSource)).string();

                assets_manager->queue_file_load(imgPath.c_str());


                assets_manager->pool_works();
            }
        }
        else {

            tsData.name = ts->Attribute("name") ? ts->Attribute("name") : "";
            ts->QueryIntAttribute("tilewidth", &tsData.tileWidth);
            ts->QueryIntAttribute("tileheight", &tsData.tileHeight);

            if (tinyxml2::XMLElement* image = ts->FirstChildElement("image")) {
                tsData.imageSource = image->Attribute("source") ? image->Attribute("source") : "";
                image->QueryIntAttribute("width", &tsData.imageWidth);
                image->QueryIntAttribute("height", &tsData.imageHeight);

                if (assets_manager && !tsData.imageSource.empty()) {
                    std::string imgPath = (fs::path(basePath) / fs::path(tsData.imageSource)).string();

                    assets_manager->queue_file_load(imgPath.c_str());

                    assets_manager->pool_works();
                }
            }
        }

        tilesets.push_back(tsData);
    }

    return true;
}
