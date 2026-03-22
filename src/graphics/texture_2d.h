//
// Created by jhone on 09/08/2025.
//

#ifndef TEXTURE_H
#define TEXTURE_H
#include "vector2.h"
#include "glad/glad.h"

class Texture2D {
    GLuint load_texture(const char *path, int &width, int &height);

    GLuint texture;
    int width, height;

public:
    explicit Texture2D(const char *path);

    explicit Texture2D(const char *path, Vector2 &apply_scale);

    ~Texture2D();

    GLuint get_texture() const;

    unsigned int get_width() const;

    unsigned int get_height() const;

    void use() const;
};


#endif //TEXTURE_H
