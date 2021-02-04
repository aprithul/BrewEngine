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
#include "Types.hpp"
#include <string>

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


        Vector3<Double_64> operator*(const Double_64 d) const{
            return Vector3<Double_64>( (Double_64)this->x * d, (Double_64)this->y * d, (Double_64)this->z * d);
        }

        Vector3<Float_32> operator*(const Float_32 f) const{
            return Vector3<Float_32>( (Float_32)this->x * f, (Float_32)this->y * f, (Float_32)this->z * f);
        }

        Vector3 operator/(const T v) const{
        return Vector3<T>( this->x / v, this->y / v, this->z / v);

        }

        Vector3 operator==(const T v ) const{
            return ( this->x == v.x && this->y == v.y && this->z == v.z);
        }

		Vector3 operator=(const T& v) {
			this->x = v.x;
			this->y = v.y;
			this->z = v.z;
			return *this;
		}

        Vector3 normalize(){
        Double_64 len = length();
        x /= len;
        y /= len;
        z /= len;
        return (*this);
        }
        Float_32 length() const{
        return sqrt( x*x + y*y + z*z);

        }

		std::string to_string()
		{
			std::string ret = std::to_string(x) + "," + std::to_string(y) + "," + std::to_string(z);
			return ret;
		}
    };
        
}
#endif /* Vector3_hpp */
