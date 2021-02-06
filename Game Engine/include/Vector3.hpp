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
	class Vector3;

	template<typename T>
	class Vector2
	{
	public:
		T x, y;

		Vector2()
		{

		}

		Vector2(T x, T y)
		{
			this->x = x;
			this->y = y;
		}

		Vector2(Vector3<T> v2)
		{
			this->x = v2.x;
			this->y = v2.y;
		}

		~Vector2()
		{

		}

		Vector2<T> operator+(const Vector2& v) const
		{
			return Vector2<T>(this->x + v.x, this->y + v.y);

		}
		Vector2<T> operator+=(const Vector2& v) {
			this->x += v.x;
			this->y += v.y;
			return *this;//Vector2<T>( this->x, this->y);
		}
		Vector2 operator-(const Vector2& v) const {
			return Vector2<T>(this->x - v.x, this->y - v.y);
		}
		T operator*(const Vector2& v) const {
			return (this->x * v.x + this->y * v.y);
		}

		Vector2<T> operator^(const Vector2& v) const {
			return Vector2<T>(this->x * v.x, this->y * v.y);
		}


		Vector2<Double_64> operator*(const Double_64 d) const {
			return Vector2<Double_64>((Double_64)this->x * d, (Double_64)this->y * d);
		}

		Vector2<Float_32> operator*(const Float_32 f) const {
			return Vector2<Float_32>((Float_32)this->x * f, (Float_32)this->y * f);
		}

		Vector2 operator/(const T v) const {
			return Vector2<T>(this->x / v, this->y / v);

		}

		explicit operator Vector2<Int_32>() const { 
			Vector2<Int_32> v = { (Int_32)x, (Int_32)y };
			return v;
		}

		explicit operator Vector2<Float_32>() const {
			Vector2<Float_32> v = { (Float_32)x, (Float_32)y };
			return v;
		}


		Vector2 normalize() {
			Double_64 len = length();
			if (len <= 0.000001)
				return (*this);
			x /= len;
			y /= len;
			return (*this);
		}
		Float_32 length() const {
			return sqrt(x*x + y * y);

		}
	};



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

		Vector3(Vector2<T> v2)
		{
			this->x = v2.x;
			this->y = v2.y;
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

		explicit operator Vector3<Int_32>() const {
			Vector3<Int_32> v = { (Int_32)x, (Int_32)y, (Int_32)z};
			return v;
		}

		explicit operator Vector3<Float_32>() const {
			Vector3<Float_32> v = { (Float_32)x, (Float_32)y, (Float_32)z};
			return v;
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
	
	template<typename T>
	struct Vector4
	{
		T x, y, z, w;

		Vector4()
		{
			this->x = 0;
			this->y = 0;
			this->z = 0;
			this->w = 0;
		}

		Vector4(T x, T y, T z, T w)
		{
			this->x = x;
			this->y = y;
			this->z = z;
			this->w = w;
		}

		Vector4(Vector3<T> v2)
		{
			this->x = v2.x;
			this->y = v2.y;
			this->z = 0;
			this->w = 0;
		}

		explicit operator Vector3<Float_32>()  const
		{
			Vector3<Float_32>v = { x, y, z };
			return v;
		}

	};


}
#endif /* Vector3_hpp */
