//
// Created by jhone on 09/08/2025.
//

#include "texture_2d.h"
#define STB_IMAGE_IMPLEMENTATION
#include <iostream>
#include <ostream>
#include <stb_image/stb_image.h>
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include <stb_image/stb_image_resize.h>


GLuint Texture2D::load_texture(const char *path, int &width, int &height) {
    int channels;

    uint8_t* data = stbi_load(path, &width, &height, &channels, 4); // 4 RGBA

    if (!data) {
        std::cerr << "Failed to load texture" << std::endl;
        return 0;
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

Texture2D::Texture2D(const char *path) {
    width=0, height=0;
    texture = load_texture(path, width, height);

}

Texture2D::Texture2D(const char *path, Vector2 &apply_scale) {
    int channels;
    int original_width, original_height;

    uint8_t* data = stbi_load(path, &original_width, &original_height, &channels, 4); // RGBA
    if (!data) {
        std::cerr << "Failed to load texture" << std::endl;
        texture = 0;
        width = height = 0;
        return;
    }

    // Calcula nova dimensão aplicando a escala
    width = static_cast<int>(original_width * apply_scale.x);
    height = static_cast<int>(original_height * apply_scale.y);

    // Aloca buffer para imagem redimensionada
    uint8_t* resized_data = new uint8_t[width * height * 4];

    // Redimensiona
    stbir_resize_uint8(data, original_width, original_height, 0,
                       resized_data, width, height, 0, 4);

    // Cria a textura
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, resized_data);
    glGenerateMipmap(GL_TEXTURE_2D);

    // Libera memória
    delete[] resized_data;
    stbi_image_free(data);
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
