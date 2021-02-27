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
#include "Types.hpp"

#ifdef __APPLE__
	#include <OpenGL/gl3.h>
#elif _WIN64
	#include <GL/glew.h>
#endif

namespace PrEngine {

    
    struct Vertex
    {
        Vertex();
        Vertex(Float_32 x,Float_32 y, Float_32 z);
        Vertex(Float_32 x, Float_32 y, Float_32 z, Float_32 nx, Float_32 ny, Float_32 nz);
        Vertex(Float_32 x, Float_32 y, Float_32 z, Float_32 r, Float_32 g, Float_32 b, Float_32 a, Float_32 nx, Float_32 ny, Float_32 nz);
        Vertex(Float_32 x, Float_32 y, Float_32 z, Float_32 r, Float_32 g, Float_32 b, Float_32 a, Float_32 nx, Float_32 ny, Float_32 nz, Float_32 u, Float_32 v);
        Float_32 p_x;
        Float_32 p_y;
        Float_32 p_z;
        Float_32 c_r;
        Float_32 c_g;
        Float_32 c_b;
        Float_32 c_a;
        Float_32 n_x;
        Float_32 n_y;
        Float_32 n_z;
        Float_32 u;
        Float_32 v;        
    };

    
}


#endif /* Vertex_hpp */
