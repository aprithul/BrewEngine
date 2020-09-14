#include "Entity3D.hpp"

namespace PrEngine{

    Entity3D::Entity3D(Graphics* graphics):graphics(graphics)
    {
        //graphics->models.push_back( &(transform.get_transformation()));
        //graphics->normals.push_back(&transform.get_rotation_transformation());
    }

    Entity3D::~Entity3D()
    {
        
    }



}