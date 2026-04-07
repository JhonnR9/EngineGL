//
// Created by jhone on 09/08/2025.
//

#ifndef TEXTURE_H
#define TEXTURE_H
#include "utils/vector2.h"
#include "glad.h"
#include "utils/asset.h"

enum class TextureFilter {
    Nearest,
    Linear,
};

class Texture2D : public Asset {
    GLuint load_texture(const char *path, int &width, int &height);

    void setup_texture();

    GLuint texture;
    int width{0}, height{0};
    TextureFilter filter {TextureFilter::Linear};

public:
    explicit Texture2D(const char *path, const TextureFilter filter = TextureFilter::Linear);

    explicit Texture2D(const std::vector<char> &bytes, TextureFilter filter = TextureFilter::Linear);

    explicit Texture2D(const char *path, Vector2 &apply_scale);

    explicit Texture2D(int width, int height, const unsigned char *data, TextureFilter filter = TextureFilter::Linear);

    ~Texture2D() override;

    GLuint get_texture() const;

    unsigned int get_width() const;

    unsigned int get_height() const;

    void use() const;
};


#endif //TEXTURE_H
