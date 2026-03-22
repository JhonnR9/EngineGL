//
// Created by jhone on 09/08/2025.
//

#ifndef MATERIAL_H
#define MATERIAL_H
#include <memory>

#include "shader.h"

class Texture2D;
class Material {
    std::unique_ptr<Shader> shader;
    Texture2D* texture;

public:
    Material()=default;
    ~Material()=default;

    GLint get_uniform_location(const char* name) const;
    void set_shader(std::unique_ptr<Shader> shader);
    void set_texture(Texture2D* texture);
    Texture2D* get_texture() const;
    Shader* get_shader() const;

    void use() const;
};


#endif //MATERIAL_H
