//
// Created by jhone on 29/03/2026.
//

#include "tsx_reader.h"
#include <iostream>

bool TSXReader::load_tsx(const char *path) {
    doc = std::make_unique<tinyxml2::XMLDocument>();
    tinyxml2::XMLError err = doc->LoadFile(path);
    if (err != tinyxml2::XML_SUCCESS) {
        std::cout << "Fail to load " << path << " !" << std::endl;
        return false;
    }
    return true;
}

bool TSXReader::parse_tsx() {
    tinyxml2::XMLElement *tileset = doc->FirstChildElement("tileset");
    if (!tileset) {
        std::cerr << "Element <tileset> not found" << std::endl;
        return false;
    }

    tilesetData.name = (tileset->Attribute("name") ? tileset->Attribute("name") : "");
    int tw = 0, th = 0;
    tileset->QueryIntAttribute("tilewidth", &tw);
    tileset->QueryIntAttribute("tileheight", &th);
    tilesetData.tileWidth = (tw);
    tilesetData.tileHeight = (th);

    if (tinyxml2::XMLElement *image = tileset->FirstChildElement("image")) {
        tilesetData.imageSource=(image->Attribute("source") ? image->Attribute("source") : "");
        int iw = 0, ih = 0;
        image->QueryIntAttribute("width", &iw);
        image->QueryIntAttribute("height", &ih);
        tilesetData.imageWidth=(iw);
        tilesetData.imageHeight=(ih);
    }

    return true;
}

TSXReader::TSXReader(const char *path) {
    if (load_tsx(path)) {
        parse_tsx();
    }
}
