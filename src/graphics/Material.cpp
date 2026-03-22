//
// Created by jhone on 09/08/2025.
//

#include "Material.h"
#include "texture_2d.h"

GLint Material::get_uniform_location(const char *name) const {
    return glGetUniformLocation(this->shader->get_program(), name);
}

void Material::set_shader(std::unique_ptr<Shader> shader) {
    this->shader = std::move(shader);
}

void Material::set_texture(Texture2D* texture) {
    this->texture = texture;
}

Texture2D * Material::get_texture() const {
    return this->texture;
}

void Material::use() const {
    shader->use();
    texture->use();
}

