//
//  Matrix4x4.hpp
//  Game Engine
//
//  Created by Aniruddha Prithul on 7/28/17.
//  Copyright © 2017 Aniruddha Prithul. All rights reserved.
//

#ifndef Matrix3x3_hpp
#define Matrix3x3_hpp

#include "Types.hpp"

namespace PrEngine {
    class Matrix3x3f
    {
    public:
        Matrix3x3f();
        ~Matrix3x3f();
        Matrix3x3f operator+(Matrix3x3f m);
        Matrix3x3f operator-(Matrix3x3f m);
        Matrix3x3f operator*(Matrix3x3f m);
        Matrix3x3f operator/(Float_32 v);
        Matrix3x3f operator*(Float_32 v);
        
        void set(Int_32 i, Int_32 j, Float_32 v);
        Float_32 get(Int_32 i, Int_32 j);
        
    private:
        Float_32 m[4][4];
    };
}

#endif /* Matrix4x4_hpp */
