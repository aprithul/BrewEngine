//
//  Vertex.cpp
//  Game Engine
//
//  Created by Aniruddha Prithul on 7/30/17.
//  Copyright © 2017 Aniruddha Prithul. All rights reserved.
//

#include "Vertex.hpp"

namespace Pringine {
            
    Vertex::Vertex()
    {
        Vector3f v(0,0,0);
        this->position = v;
    }
    
    Vertex::Vertex(GLfloat x, GLfloat y, GLfloat z)
    {
        Vector3f v( x, y, z);
        this->position = v;
    }
    
    Vertex::Vertex(Vector3f position)
    {
        this->position = position;
    }
    
    //NEEDS TO BE EDITED WHEN NEW ATTRIBUTES ARE ADDED IN THE FUTURE
    GLfloat* Vertex::get_flattened_attributes_array()
    {
        flattened[0] = this->position.get_x();
        flattened[1] = this->position.get_y();
        flattened[2] = this->position.get_z();
        return flattened;
    }

    
}
