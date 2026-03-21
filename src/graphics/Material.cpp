//
// Created by jhone on 09/08/2025.
//

#include "Material.h"

GLint Material::get_uniform_location(const char *name) const {
    return glGetUniformLocation(this->shader->get_program(), name);
}

void Material::set_shader(std::unique_ptr<Shader> shader) {
    this->shader = std::move(shader);
}

void Material::set_texture(std::unique_ptr<Texture> texture) {
    this->texture = std::move(texture);
}

Texture & Material::get_texture() const {
    return *this->texture;
}

void Material::use() const {
    shader->use();
    texture->use();
}

