#include "font.h"
#include <ft2build.h>
#include <stdexcept>
#include <algorithm>
#include <cmath>

#include FT_FREETYPE_H

void Font::calculate_size() {
    const int size = std::sqrt(ascii_characters * (font_size * font_size));
    atlas_width = size * 2;
    atlas_height = size * 2;
}

Font::Font(const std::string &path, int fontSize) {
    font_size = fontSize;

    calculate_size();

    atlas_buffer.resize(atlas_width * atlas_height * 4, 0); // RGBA

    FT_Library ft;
    if (FT_Init_FreeType(&ft)) {
        throw std::runtime_error("Error initializing FreeType");
    }

    FT_Face face;
    if (FT_New_Face(ft, path.c_str(), 0, &face)) {
        throw std::runtime_error("Erro ao carregar fonte");
    }

    FT_Set_Pixel_Sizes(face, 0, font_size);


    FT_Size_Metrics metrics = face->size->metrics;
    ascent  = metrics.ascender >> 6;
    descent = metrics.descender >> 6;
    line_height = metrics.height >> 6;

    int x = 0;
    int y = 0;
    int row_height = 0;

    const int padding = 2;

    for (unsigned char c = 0; c < ascii_characters; c++) {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER | FT_LOAD_FORCE_AUTOHINT)) {
            continue;
        }

        FT_GlyphSlot g = face->glyph;

        int gw = g->bitmap.width;
        int gh = g->bitmap.rows;


        if (x + gw + padding * 2 >= atlas_width) {
            x = 0;
            y += row_height;
            row_height = 0;
        }


        for (int row = 0; row < gh; row++) {
            for (int col = 0; col < gw; col++) {
                int glyph_index = col + row * g->bitmap.pitch;
                unsigned char value = g->bitmap.buffer[glyph_index];

                int atlas_x = x + col + padding;
                int atlas_y = y + row + padding;

                int atlas_index = (atlas_x + atlas_y * atlas_width) * 4;

                atlas_buffer[atlas_index + 0] = 255;
                atlas_buffer[atlas_index + 1] = 255;
                atlas_buffer[atlas_index + 2] = 255;
                atlas_buffer[atlas_index + 3] = value;
            }
        }

        Character ch;

        ch.uv_min = glm::vec2(
            float(x + padding) / atlas_width,
            float(y + padding) / atlas_height
        );

        ch.uv_max = glm::vec2(
            float(x + padding + gw) / atlas_width,
            float(y + padding + gh) / atlas_height
        );

        ch.size = glm::ivec2(gw, gh);
        ch.bearing = glm::ivec2(g->bitmap_left, g->bitmap_top);


        ch.advance = g->advance.x >> 6;

        characters.insert({c, ch});

        x += gw + padding * 2;
        row_height = std::max(row_height, gh + padding * 2);
    }

    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    atlas_texture = std::make_shared<Texture2D>(atlas_width, atlas_height, atlas_buffer.data());
}
