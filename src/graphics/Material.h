//
// Created by jhone on 09/08/2025.
//

#ifndef MATERIAL_H
#define MATERIAL_H
#include <memory>

#include "Shader.h"
#include "Texture.h"


class Material {
    std::unique_ptr<Shader> shader;
    std::unique_ptr<Texture> texture;

public:
    Material()=default;
    ~Material()=default;

    GLint get_uniform_location(const char* name) const;
    void set_shader(std::unique_ptr<Shader> shader);
    void set_texture(std::unique_ptr<Texture> texture);
    Texture& get_texture() const;

    void use() const;
};


#endif //MATERIAL_H
