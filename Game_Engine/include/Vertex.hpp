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
#include "Vec.hpp"
#include "Types.hpp"

#ifdef __APPLE__
	#include <OpenGL/gl3.h>
#elif _WIN64
	#include <GL/glew.h>
#endif

namespace PrEngine {

    
    struct Vertex // 40 bytes
    {
        Float_32 p_x;
        Float_32 p_y;
        Float_32 p_z;
		Float_32 c_r;
		Float_32 c_g;
		Float_32 c_b;
		Float_32 c_a;
        Float_32 u;
        Float_32 v;  
		Float_32 m_id;
    };

    
}


#endif /* Vertex_hpp */
