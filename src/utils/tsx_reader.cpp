#include "tsx_reader.h"
#include <iostream>

TSXReader::TSXReader(const std::vector<char>& raw_data) {
    doc = std::make_shared<tinyxml2::XMLDocument>();
    if (doc->Parse(raw_data.data(), raw_data.size()) == tinyxml2::XML_SUCCESS) {
        parse_tsx();
    }
}

TSXReader::TSXReader(const char *path) {
    doc = std::make_shared<tinyxml2::XMLDocument>();
    if (doc->LoadFile(path) == tinyxml2::XML_SUCCESS) {
        parse_tsx();
    }
}

bool TSXReader::parse_tsx() {
    tinyxml2::XMLElement *tileset = doc->FirstChildElement("tileset");
    if (!tileset) return false;

    tilesetData.name = (tileset->Attribute("name") ? tileset->Attribute("name") : "");
    tileset->QueryIntAttribute("tilewidth", &tilesetData.tileWidth);
    tileset->QueryIntAttribute("tileheight", &tilesetData.tileHeight);

    if (tinyxml2::XMLElement *image = tileset->FirstChildElement("image")) {
        tilesetData.imageSource = (image->Attribute("source") ? image->Attribute("source") : "");
        image->QueryIntAttribute("width", &tilesetData.imageWidth);
        image->QueryIntAttribute("height", &tilesetData.imageHeight);
    }
    return true;
}