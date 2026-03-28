//
// Created by jhone on 27/03/2026.
//

#ifndef FONT_H
#define FONT_H

#include <memory>
#include <string>
#include <unordered_map>

#include "texture_2d.h"
#include "glm/exponential.hpp"
#include "glm/vec2.hpp"

struct Character {
    glm::vec2 uv_min;
    glm::vec2 uv_max;
    glm::ivec2 size;
    glm::ivec2 bearing;
    unsigned int advance;
};

class Font {
    int font_size = 16;
    int atlas_width = 0;
    int atlas_height = 0;
    int ascii_characters = 128;


    void calculate_size();

public:
    Font(const std::string &path, int fontSize);
    std::shared_ptr<Texture2D> atlas_texture;
    std::unordered_map<char, Character> characters;
    std::vector<unsigned char> atlas_buffer;

};

#endif //FONT_H
