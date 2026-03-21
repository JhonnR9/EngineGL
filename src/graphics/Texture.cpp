//
// Created by jhone on 09/08/2025.
//

#include "Texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include <iostream>
#include <ostream>
#include <stb_image/stb_image.h>

GLuint Texture::load_texture(const char *path, int &width, int &height) {
    int channels;

    uint8_t* data = stbi_load(path, &width, &height, &channels, 4); // 4 RGBA

    if (!data) {
        std::cerr << "Failed to load texture" << std::endl;
    }

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);

    return texture;
}

Texture::Texture(const char *path) {
    width=0, height=0;
    texture = load_texture(path, width, height);

}

Texture::~Texture() {
    if (texture) {
        glDeleteTextures(1, &texture);
    }
}

GLuint Texture::get_texture() const {
    return texture;
}

unsigned int Texture::get_width() const {
    return width;
}

unsigned int Texture::get_height() const {
    return height;
}

void Texture::use() const {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
}
