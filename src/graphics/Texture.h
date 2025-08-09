//
// Created by jhone on 09/08/2025.
//

#ifndef TEXTURE_H
#define TEXTURE_H
#include "glad/glad.h"

class Texture {
    [[nodiscard]] GLuint load_texture(const char *path, int &width, int &height);
    GLuint texture;
    int width, height;

    public:
    explicit Texture(const char *path);
    ~Texture();
    [[nodiscard]] GLuint get_texture() const;
    [[nodiscard]] unsigned int get_width() const;
    [[nodiscard]] unsigned int get_height() const;
    void use() const;
};


#endif //TEXTURE_H
