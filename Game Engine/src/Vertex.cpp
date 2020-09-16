//
//  Vertex.cpp
//  Game Engine
//
//  Created by Aniruddha Prithul on 7/30/17.
//  Copyright © 2017 Aniruddha Prithul. All rights reserved.
//

#include "Vertex.hpp"

namespace PrEngine {

    Vertex::Vertex()
    {
        p_x = 0.f;
        p_y = 0.f;
        p_z = 0.f;
        c_r = 0.f;
        c_g = 0.f;
        c_b = 0.f;
        c_a = 0.f;
        n_x = 0.f;
        n_y = 0.f;
        n_z = 0.f;
        u = 0.f;
        v = 0.f;
    }

    Vertex::Vertex(float x, float y, float z)
    {
        p_x = x;
        p_y = y;
        p_z = z;
        c_r = 0.f;
        c_g = 0.f;
        c_b = 0.f;
        c_a = 0.f;
        n_x = 0.f;
        n_y = 0.f;
        n_z = 0.f;
        u = 0.f;
        v = 0.f;
    }


    Vertex::Vertex(float x, float y, float z, float nx, float ny, float nz)
    {
        p_x = x;
        p_y = y;
        p_z = z;
        c_r = 0.f;
        c_g = 0.f;
        c_b = 0.f;
        c_a = 0.f;
        n_x = nx;
        n_y = ny;
        n_z = nz;
        u = 0.f;
        v = 0.f;
    }


    Vertex::Vertex(float x, float y, float z, float nx, float ny, float nz, float r, float g, float b, float a)
    {
        p_x = x;
        p_y = y;
        p_z = z;
        c_r = r;
        c_g = g;
        c_b = b;
        c_a = a;
        n_x = nx;
        n_y = ny;
        n_z = nz;
        u = 0.f;
        v = 0.f;
    }

    Vertex::Vertex(float x, float y, float z, float nx, float ny, float nz, float r, float g, float b, float a, float u, float v)
    {
        p_x = x;
        p_y = y;
        p_z = z;
        c_r = r;
        c_g = g;
        c_b = b;
        c_a = a;
        n_x = nx;
        n_y = ny;
        n_z = nz;
        this->u = u;
        this->v = v;
    }


}
