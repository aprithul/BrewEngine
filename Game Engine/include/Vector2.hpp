//
//  Vector2.hpp
//  Game Engine
//
//  Created by Aniruddha Prithul on 7/23/17.
//  Copyright © 2017 Aniruddha Prithul. All rights reserved.
//

#ifndef VECTOR2_HPP
#define VECTOR2_HPP

#include <stdio.h>
#include "math.h"
#include "Types.hpp"

namespace PrEngine {

    template<typename T>
    class Vector2
    {
    public:
        T x,y;
        
        Vector2()
        {

        }

        Vector2(T x, T y)
        {
            this->x = x;
            this->y = y;
        }
        ~Vector2()
        {

        }

        Vector2<T> operator+(const Vector2& v) const
        {
            return Vector2<T>( this->x + v.x, this->y + v.y);

        }
        Vector2<T> operator+=(const Vector2& v){
            this->x += v.x;
            this->y += v.y;
            return *this;//Vector2<T>( this->x, this->y);
        }
        Vector2 operator-(const Vector2& v) const{
            return Vector2<T>( this->x - v.x, this->y - v.y);
        }
        T operator*(const Vector2& v) const{
            return (this->x * v.x + this->y * v.y);
        }

        Vector2<T> operator^(const Vector2& v) const{
            return Vector2<T>(this->x * v.x, this->y * v.y);
        }


        Vector2<Double_64> operator*(const Double_64 d) const{
            return Vector2<Double_64>( (Double_64)this->x * d, (Double_64)this->y * d);
        }

        Vector2<Float_32> operator*(const Float_32 f) const{
            return Vector2<Float_32>( (Float_32)this->x * f, (Float_32)this->y * f);
        }

        Vector2 operator/(const T v) const{
        return Vector2<T>( this->x / v, this->y / v);

        }
        Vector2 normalize(){
        Double_64 len = length();
        if(len<=0.000001)
        	return (*this);
		x /= len;
        y /= len;
        return (*this);
        }
        Float_32 length() const{
        return sqrt( x*x + y*y);

        }
    };
        
}
#endif /* Vector2_hpp */
