//
// Created by jhone on 09/08/2025.
//

#ifndef MATERIAL_H
#define MATERIAL_H
#include <memory>

#include "Shader.h"
#include "Texture.h"


class Material {
std::unique_ptr<Shader> Shader;
std::unique_ptr<Texture> Texture;

    public:
    Material();
    ~Material();
};



#endif //MATERIAL_H
