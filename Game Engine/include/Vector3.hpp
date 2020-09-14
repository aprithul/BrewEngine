//
//  Vector3.hpp
//  Game Engine
//
//  Created by Aniruddha Prithul on 7/23/17.
//  Copyright © 2017 Aniruddha Prithul. All rights reserved.
//

#ifndef Vector3_HPP
#define Vector3_HPP

#include <stdio.h>
#include "math.h"

namespace PrEngine {

    template<typename T>
    class Vector3
    {
    public:
        T x,y,z;
        
        Vector3()
        {
            this->x = 0;
            this->y = 0;
            this->z = 0;
        }

        Vector3(T x, T y, T z)
        {
            this->x = x;
            this->y = y;
            this->z = z;
        }
        ~Vector3()
        {

        }

        Vector3<T> operator+(const Vector3& v) const
        {
            return Vector3<T>( this->x + v.x, this->y + v.y, this->z + v.z);

        }
        Vector3<T> operator+=(const Vector3& v){
            this->x += v.x;
            this->y += v.y;
            this->z += v.z;
            return *this;//Vector3<T>( this->x, this->y);
        }
        Vector3 operator-(const Vector3& v) const{
            return Vector3<T>( this->x - v.x, this->y - v.y, this->z - v.z);
        }
        T operator*(const Vector3& v) const{
            return (this->x * v.x + this->y * v.y + this->z * v.z);
        }

        Vector3<T> operator^(const Vector3& v) const{
            return Vector3<T>(this->x * v.x, this->y * v.y, this->z * v.z);
        }


        Vector3<double> operator*(const double d) const{
            return Vector3<double>( (double)this->x * d, (double)this->y * d, (double)this->z * d);
        }

        Vector3<float> operator*(const float f) const{
            return Vector3<float>( (float)this->x * f, (float)this->y * f, (float)this->z * f);
        }

        Vector3 operator/(const T v) const{
        return Vector3<T>( this->x / v, this->y / v, this->z / v);

        }

        Vector3 operator==(const T v ) const{
            return ( this->x == v.x && this->y == v.y && this->z == v.z);
        }

        Vector3 normalize(){
        double len = length();
        x /= len;
        y /= len;
        z /= len;
        return (*this);
        }
        float length() const{
        return sqrt( x*x + y*y + z*z);

        }
    };
        
}
#endif /* Vector3_hpp */
