//
//  Matrix4x4.hpp
//  Game Engine
//
//  Created by Aniruddha Prithul on 7/28/17.
//  Copyright © 2017 Aniruddha Prithul. All rights reserved.
//

#ifndef Matrix3x3_hpp
#define Matrix3x3_hpp


namespace PrEngine {
    class Matrix3x3f
    {
    public:
        Matrix3x3f();
        ~Matrix3x3f();
        Matrix3x3f operator+(Matrix3x3f m);
        Matrix3x3f operator-(Matrix3x3f m);
        Matrix3x3f operator*(Matrix3x3f m);
        Matrix3x3f operator/(float v);
        Matrix3x3f operator*(float v);
        
        void set(int i, int j, float v);
        float get(int i, int j);
        
    private:
        float m[4][4];
    };
}

#endif /* Matrix4x4_hpp */
