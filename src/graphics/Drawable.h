//
// Created by jhone on 09/08/2025.
//

#ifndef DRAWABLE_H
#define DRAWABLE_H

#include "Material.h"
#include "Mesh.h"


class Drawable {
    public:
    virtual ~Drawable() = default;

    virtual Mesh& get_mesh_info();
    virtual Material& get_material();
};



#endif //DRAWABLE_H
