//
//  Matrix4x4.cpp
//  Game Engine
//
//  Created by Aniruddha Prithul on 7/28/17.
//  Copyright © 2017 Aniruddha Prithul. All rights reserved.
//

#include "Matrix3x3f.hpp"

namespace PrEngine {
    Matrix3x3f::Matrix3x3f()
    {   
        
    }
    
    Matrix3x3f::~Matrix3x3f()
    {
        
    }
    
    Float_32 Matrix3x3f::get(Int_32 i, Int_32 j)
    {
        return m[i][j];
    }
    
    void Matrix3x3f::set(Int_32 i, Int_32 j, Float_32 v)
    {
        m[i][j] = v;
    }
    
    Matrix3x3f Matrix3x3f::operator*(Matrix3x3f _m)
    {
        Matrix3x3f r;
        for(Int_32 i=0; i<4; i++)
            for(Int_32 j=0; j<4; j++)
            {
                r.set( i, j,  m[i][0]*_m.get(0,j) + m[i][1]*_m.get(1,j) + m[i][2]*_m.get(2,j) + m[i][3]*_m.get(3,j));
            }
        return r;
    }
    
    Matrix3x3f Matrix3x3f::operator*(Float_32 v)
    {
        Matrix3x3f r;
        for(Int_32 i=0; i<4; i++)
            for(Int_32 j=0; j<4; j++)
            {
                r.set( i, j, m[i][j]* v);
            }
        return r;
    }

    Matrix3x3f Matrix3x3f::operator/(Float_32 v)
    {
        Matrix3x3f r;
        for(Int_32 i=0; i<4; i++)
            for(Int_32 j=0; j<4; j++)
            {
                r.set( i, j, m[i][j] / v);
            }
        return r;
    }
    
    Matrix3x3f Matrix3x3f::operator-(Matrix3x3f _m)
    {
        Matrix3x3f r;
        for(Int_32 i=0; i<4; i++)
            for(Int_32 j=0; j<4; j++)
            {
                r.set( i, j, m[i][j] - _m.get(i,j));
            }
        return r;
        
    }
    
    Matrix3x3f Matrix3x3f::operator+(Matrix3x3f _m)
    {
        Matrix3x3f r;
        for(Int_32 i=0; i<4; i++)
            for(Int_32 j=0; j<4; j++)
            {
                r.set( i, j, m[i][j] + _m.get(i,j));
            }
        return r;
        
    }
    
}
