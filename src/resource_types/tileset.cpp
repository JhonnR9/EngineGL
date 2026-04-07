//
// Created by jhone on 06/04/2026.
//

#include "tileset.h"
#include <stdexcept>
#include <tinyxml2.h>

using namespace tinyxml2;
Tileset::Tileset(const std::vector<char> &raw_data) {
    parse(raw_data);
}

void Tileset::parse(const std::vector<char> &raw_data) {
    XMLDocument doc;
    if(doc.Parse(raw_data.data(), raw_data.size()) == tinyxml2::XML_SUCCESS){
        XMLElement *tileset = doc.FirstChildElement("tileset");
        if (!tileset) {
            throw std::runtime_error("Failed to parse tileset");
        }

        name = (tileset->Attribute("name") ? tileset->Attribute("name") : "");
        tileset->QueryIntAttribute("tilewidth", &tile_width);
        tileset->QueryIntAttribute("tileheight", &tile_height);

        if (XMLElement *image = tileset->FirstChildElement("image")) {
            image_source = (image->Attribute("source") ? image->Attribute("source") : "");
            image->QueryIntAttribute("width", &image_width);
            image->QueryIntAttribute("height", &image_height);
        }

    }

}
