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
#include <memory>
#include "Vector3.hpp"

#ifdef __APPLE__
	#include <OpenGL/gl3.h>
#elif _WIN64
	#include <GL/glew.h>
#endif

namespace PrEngine {

    
    struct Vertex
    {
        Vertex();
        Vertex(float x,float y, float z);
        Vertex(float x, float y, float z, float nx, float ny, float nz);
        Vertex(float x, float y, float z, float nx, float ny, float nz, float r, float g, float b, float a);
        Vertex(float x, float y, float z, float nx, float ny, float nz, float r, float g, float b, float a, float u, float v);
        float p_x;
        float p_y;
        float p_z;
        float c_r;
        float c_g;
        float c_b;
        float c_a;
        float n_x;
        float n_y;
        float n_z;
        float u;
        float v;        
    };

    
}


#endif /* Vertex_hpp */
