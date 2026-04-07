//
// Created by jhone on 06/04/2026.
//

#include "tilemap.h"
#include <stdexcept>
#include "tinyxml2.h"
using namespace tinyxml2;

Tilemap::Tilemap(const std::vector<char> &raw_data) {
    parse(raw_data);
}

void Tilemap::parse(const std::vector<char> &raw_data) {
    XMLDocument doc;
    if (doc.Parse(raw_data.data(), raw_data.size()) == XML_SUCCESS) {
        XMLElement *map = doc.FirstChildElement("map");
        if (!map) {
            throw std::runtime_error("map element not found tmx map is corrupted");
        }
        map->QueryIntAttribute("width", &map_width);
        map->QueryIntAttribute("height", &map_height);


        XMLElement *layer = map->FirstChildElement("layer");
        while (layer) {
            TilemapLayer l;
            layer->QueryIntAttribute("id", &l.id);

            const char *name = layer->Attribute("name");
            l.name = name ? name : "";

            XMLElement *csv_dta = layer->FirstChildElement("data");

            if (auto csv = csv_dta->GetText()) {
                l.csv_data = csv;
            }

            layer = layer->NextSiblingElement("layer");
        }
    }
}
