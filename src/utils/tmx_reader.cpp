#include "tmx_reader.h"
#include <iostream>
#include <filesystem>
namespace fs = std::filesystem;

bool TMXReader::load_tmx(const char *path) {
    fs::path tmxpath(path);

    basePath = tmxpath.parent_path().string();
    doc = std::make_unique<tinyxml2::XMLDocument>();
    tinyxml2::XMLError err = doc->LoadFile(path);
    if (err != tinyxml2::XML_SUCCESS) {
        std::cout << "Fail to load " << path << " !" << std::endl;
        return false;
    }
    return true;
}

bool TMXReader::parse_tmx() {
    tinyxml2::XMLElement* map = doc->FirstChildElement("map");
    if (!map) {
        std::cerr << "Element <map> not found" << std::endl;
        return false;
    }

    map->QueryIntAttribute("width", &mapData.width);
    map->QueryIntAttribute("height", &mapData.height);

    if (tinyxml2::XMLElement* layer = map->FirstChildElement("layer")) {
        mapData.layerName = layer->Attribute("name") ? layer->Attribute("name") : "";
        tinyxml2::XMLElement* data = layer->FirstChildElement("data");
        mapData.layerData = data && data->GetText() ? data->GetText() : "";
    }

    for (tinyxml2::XMLElement* ts = map->FirstChildElement("tileset"); ts; ts = ts->NextSiblingElement("tileset")) {
        TSXReader::TilesetData tsData;

        const char* source = ts->Attribute("source");
        if (source) {
            fs::path tsxpath = fs::path(basePath) / fs::path(source);
            TSXReader tsx(tsxpath.string().c_str());
            tsData = tsx.getTilesetData();
        } else {
            tsData.name = ts->Attribute("name") ? ts->Attribute("name") : "";
            ts->QueryIntAttribute("tilewidth", &tsData.tileWidth);
            ts->QueryIntAttribute("tileheight", &tsData.tileHeight);

            if (tinyxml2::XMLElement* image = ts->FirstChildElement("image")) {
                tsData.imageSource = image->Attribute("source") ? image->Attribute("source") : "";
                image->QueryIntAttribute("width", &tsData.imageWidth);
                image->QueryIntAttribute("height", &tsData.imageHeight);
            }
        }

        tilesets.push_back(tsData);
    }

    return true;
}

TMXReader::TMXReader(const char *path) {
    if (load_tmx(path)) {
        parse_tmx();
    }
}
