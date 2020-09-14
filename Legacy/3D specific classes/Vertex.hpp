//
//  Vertex.hpp
//  Game Engine
//
//  Created by Aniruddha Prithul on 7/30/17.
//  Copyright © 2017 Aniruddha Prithul. All rights reserved.
//

#ifndef Vertex_hpp
#define Vertex_hpp

#include <stdio.h>
#include "Vector3f.hpp"
#include <OpenGL/gl3.h>

namespace Pringine {
    
    enum ATTRIBUTE_INDEX
    {
        ATTIRB_POSITION = 0,
    };
    
    class Vertex
    {
    public:
        Vertex();
        Vertex(Vector3f position);
        Vertex(GLfloat  x,GLfloat y, GLfloat z);
        GLfloat* get_flattened_attributes_array(); // returns a pointer to an array that contains all attributes layed out sequentially
        
        // need to change with more attributes added
        static const GLint NUM_OF_ATTRIB = 1;
        static const GLint ATTRIB_POSITION_LENGTH = 3;
        static const GLint VERTEX_LENGTH = 3;
        static const GLint SIZE = sizeof(GLfloat)*Vertex::VERTEX_LENGTH;
        //-------------------------------------------------------------------------------------------//
        
        Vector3f position;
    private:
        GLfloat flattened[3];
    };
}


#endif /* Vertex_hpp */
