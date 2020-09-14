#ifndef ENTITY_3D_HPP
#define ENTITY_3D_HPP

#include "Entity.hpp"
#include "RendererOpenGL.hpp"
#include "Transform3D.hpp"

namespace PrEngine
{
    class Entity3D : public Entity
    {
        public:
            Transform3D transform;
            Graphics* graphics;

            Entity3D(Graphics* graphics);
            ~Entity3D();


    };
}

#endif