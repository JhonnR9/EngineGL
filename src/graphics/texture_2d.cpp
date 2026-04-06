//
// Created by jhone on 09/08/2025.
//

#include "texture_2d.h"
#define STB_IMAGE_IMPLEMENTATION
#include <iostream>
#include <ostream>
#include <stb_image.h>
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "stb_image_resize2.h"


GLuint Texture2D::load_texture(const char *path, int &width, int &height) {
    int channels;

    uint8_t *data = stbi_load(path, &width, &height, &channels, 4); // 4 RGBA

    if (!data) {
        std::cerr << "Failed to load texture" << std::endl;
        return 0;
    }

    setup_texture();


    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);

    return texture;
}

void Texture2D::setup_texture() {
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    if (filter == TextureFilter::Linear) {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    } else if (filter == TextureFilter::Nearest) {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    }

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);


    glGenerateMipmap(GL_TEXTURE_2D);
}

Texture2D::Texture2D(const char *path, const TextureFilter filter) {
    this->filter = filter;
    texture = load_texture(path, width, height);
}

Texture2D::Texture2D(const std::vector<char> &bytes, const TextureFilter filter): filter(filter), texture(0), width(0),
    height(0) {
    int channels;

    uint8_t *data = stbi_load_from_memory(
        reinterpret_cast<const unsigned char *>(bytes.data()),
        static_cast<int>(bytes.size()),
        &width,
        &height,
        &channels,
        4
    );

    if (!data) {
        throw std::runtime_error("Failed to make texture from bytes memory");
    }

    setup_texture();

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);

}

Texture2D::Texture2D(const char *path, Vector2 &apply_scale) {
    int channels;
    int original_width, original_height;

    uint8_t *data = stbi_load(path, &original_width, &original_height, &channels, 4); // RGBA
    if (!data) {
        std::cerr << "Failed to load texture \"" << path << "\"" << std::endl;
        texture = 0;
        width = height = 0;
        return;
    }


    width = static_cast<int>(original_width * apply_scale.x);
    height = static_cast<int>(original_height * apply_scale.y);


    uint8_t *resized_data = new uint8_t[width * height * 4];

    stbir_pixel_layout layout = {};

    stbir_resize_uint8_linear(data, original_width, original_height, 0,
                              resized_data, width, height, 0, layout);


    setup_texture();
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, resized_data);

    delete[] resized_data;
    stbi_image_free(data);
}

Texture2D::Texture2D(int width, int height, const unsigned char *data) {
    this->width = width;
    this->height = height;

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGBA,
        width,
        height,
        0,
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        data
    );
}


Texture2D::~Texture2D() {
    if (texture) {
        glDeleteTextures(1, &texture);
        texture = 0;
    }
}

GLuint Texture2D::get_texture() const {
    return texture;
}

unsigned int Texture2D::get_width() const {
    return width;
}

unsigned int Texture2D::get_height() const {
    return height;
}

void Texture2D::use() const {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
}
