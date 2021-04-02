//
//  Vector3.hpp
//  Game Engine
//
//  Created by Aniruddha Prithul on 7/23/17.
//  Copyright © 2017 Aniruddha Prithul. All rights reserved.
//

#ifndef Vector3_HPP
#define Vector3_HPP
#define PI 3.14159
#define MIN_VEC_LEN 0.000001
#include <stdio.h>
#include <cmath>
#include "Platform.hpp"
#include <string>

namespace PrEngine {



	template<typename T>
	class Vector2;

	template<typename T>
	class Vector3
	{
	public:
		T x, y, z;

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
			return Vector3<T>(this->x + v.x, this->y + v.y, this->z + v.z);

		}
		Vector3<T> operator+=(const Vector3& v) {
			this->x += v.x;
			this->y += v.y;
			this->z += v.z;
			return *this;//Vector3<T>( this->x, this->y);
		}

		Vector3<T> operator-=(const Vector3& v) {
			this->x -= v.x;
			this->y -= v.y;
			this->z -= v.z;
			return *this;//Vector3<T>( this->x, this->y);
		}

		Vector3 operator-(const Vector3& v) const {
			return Vector3<T>(this->x - v.x, this->y - v.y, this->z - v.z);
		}

		Vector3<T> operator-() const {
			return Vector3<T>(-this->x, -this->y, -this->z);
		}

		T operator*(const Vector3& v) const {
			return (this->x * v.x + this->y * v.y + this->z * v.z);
		}

		Vector3<T> operator^(const Vector3& v) const {
			return Vector3<T>((this->y * v.z - this->z * v.y),
				-(this->x * v.z - this->z * v.x),
				(this->x * v.y - this->y * v.x));
		}


		Vector3<Double_64> operator*(const Double_64 d) const {
			return Vector3<Double_64>((Double_64)this->x * d, (Double_64)this->y * d, (Double_64)this->z * d);
		}

		Vector3<Float_32> operator*(const Float_32 f) const {
			return Vector3<Float_32>((Float_32)this->x * f, (Float_32)this->y * f, (Float_32)this->z * f);
		}

		Vector3 operator/(const T v) const {
			return Vector3<T>(this->x / v, this->y / v, this->z / v);

		}

		Vector3 operator==(const T v) const {
			return (this->x == v.x && this->y == v.y && this->z == v.z);
		}

		Vector3 operator=(const T& v) {
			this->x = v.x;
			this->y = v.y;
			this->z = v.z;
			return *this;
		}

		explicit operator Vector3<Int_32>() const {
			Vector3<Int_32> v = { (Int_32)x, (Int_32)y, (Int_32)z };
			return v;
		}

		explicit operator Vector3<Float_32>() const {
			Vector3<Float_32> v = { (Float_32)x, (Float_32)y, (Float_32)z };
			return v;
		}

		Vector3 normalize() {
			Double_64 len = length();
			if (len < MIN_VEC_LEN)
			{
				x = 0;
				y = 0;
				z = 0;
			}
			else
			{
				x /= len;
				y /= len;
				z /= len;
			}

			return (*this);
		}
		Float_32 length() const {
			return sqrt(x*x + y * y + z * z);

		}

		Float_32 sqrd_length() const {
			return x * x + y * y + z * z;

		}


		std::string to_string()
		{
			std::string ret = std::to_string(x) + "," + std::to_string(y) + "," + std::to_string(z);
			return ret;
		}

		static Float_32 distance(Vector3<T> a, Vector3<T> b)
		{
			Float_32 dist = 0;
			dist = (a.x - b.x)*(a.x - b.x) + (a.y - b.y)*(a.y - b.y) + (a.z - b.z)*(a.z - b.z);
			dist = std::sqrt(dist);
			return dist;
		}
	};

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
			this->x = 0;
			this->y = 0;
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

		Vector2<T> operator-() const {
			return Vector2<T>{-this->x, -this->y};
		}

		T operator*(const Vector2& v) const {
			return (this->x * v.x + this->y * v.y);
		}

		Vector2<T> operator^(const Vector2& v) const {
			return Vector2<T>(this->x * v.x, this->y * v.y);
		}


		Vector2<T> operator*(const T d) const {
			return Vector2<T>((T)this->x * d, (T)this->y * d);
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
			if (len < MIN_VEC_LEN)
			{
				x = 0;
				y = 0;
			}
			else
			{
				x /= len;
				y /= len;
			}

			return (*this);
		}
		Float_32 length() const {
			return sqrt(x*x + y * y);

		}

		static Float_32 distance(Vector2<T> a, Vector2<T> b)
		{
			Float_32 dist = 0;
			dist = (a.x - b.x)*(a.x - b.x) + (a.y - b.y)*(a.y - b.y);
			dist = std::sqrt(dist);
			return dist;
		}

		static Float_32 angle(Vector2<T> dir_1, Vector2<T> dir_2)
		{
			//if (dir_1.x < 0.0001)dir_1.x = 0;
			//if (dir_1.y < 0.0001)dir_1.y = 0;
			Vector3<Float_32> dir_1_3 = dir_1;
			Vector3<Float_32> dir_2_3 = dir_2;
			Vector3<Float_32> _dir_3 = dir_1_3 ^ dir_2_3;

			dir_1.normalize();
			dir_2.normalize();
			Float_32 _val = dir_1 * dir_2;
#ifdef  _SWITCH
			Float_32 a = std::acosh(dir_1 * dir_2)*(180.f / PI);
#elif _WIN64
			Float_32 a = std::acosf(dir_1 * dir_2)*(180.f / PI);
#endif
			
			Int_32 _sign = std::signbit(_dir_3.z) ? 1 : -1;

#ifdef  _SWITCH
			Float_32 _angle = std::fmod((360.f - (_sign  * a)), 360.f);
#elif _WIN64
			Float_32 _angle = std::fmodf((360.f - (_sign  * a)), 360.f);
#endif
			return _angle;
		}

		std::string to_string()
		{
			char buffer[128];
			Float_32 x_f = this->x;
			Float_32 y_f = this->y;
			sprintf(buffer, "%.5f,%.5f", x_f, y_f);
			return std::string(buffer);
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
