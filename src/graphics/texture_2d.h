//
// Created by jhone on 09/08/2025.
//

#ifndef TEXTURE_H
#define TEXTURE_H
#include "glad/glad.h"

class Texture2D {
    GLuint load_texture(const char *path, int &width, int &height);

    GLuint texture;
    int width, height;

public:
    explicit Texture2D(const char *path);

    ~Texture2D();

    GLuint get_texture() const;

    unsigned int get_width() const;

    unsigned int get_height() const;

    void use() const;
};


#endif //TEXTURE_H
