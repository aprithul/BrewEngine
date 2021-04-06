#ifndef PRI_VEC_HPP
#define PRI_VEC_HPP

#ifndef Float_32
#define Float_32 float
#endif // !Float_32

struct Vec2f
{
	Float_32 x, y;

	//const, dest, copy
	Vec2f();
	~Vec2f() = default;
	Vec2f(Float_32, Float_32);
	Vec2f(const Vec2f& other) = default;
	Vec2f& operator=(const Vec2f& other) = default;
	Vec2f(Vec2f&& other) = default;
	Vec2f& operator=(Vec2f&& other) = delete;

	//operations
	Vec2f operator+(const Vec2f& other) const;
	Vec2f operator-(const Vec2f& other) const;
	Vec2f& operator+=(const Vec2f& other);
	Vec2f& operator-=(const Vec2f& other);
	Vec2f operator*(const Vec2f& other) const; // componenet wise
	Vec2f& operator*=(const Vec2f& other); // 
	Vec2f operator*(Float_32 s) const; // componenet wise
	Vec2f& operator*=(Float_32 s); // 
	Vec2f operator/(Float_32 s) const; 
	Vec2f& operator/=(Float_32 s);  
	Vec2f GetNormalized() const;
	Vec2f& Normalize();
	Float_32 GetMagnitude() const;
	Float_32 GetSqrdMagnitude() const;
};

struct Vec3f
{
	Float_32 x, y, z;

	//const, dest, copy
	Vec3f();
	~Vec3f() = default;
	Vec3f(Float_32 _x, Float_32 _y, Float_32 _z);
	Vec3f(const Vec3f& other) = default;
	Vec3f& operator=(const Vec3f& other) = default;
	Vec3f(Vec3f&& other) = default;
	Vec3f& operator=(Vec3f&& other) = delete;

	//operations
	Vec3f operator+(const Vec3f& other) const;
	Vec3f operator-(const Vec3f& other) const;
	Vec3f& operator+=(const Vec3f& other);
	Vec3f& operator-=(const Vec3f& other);
	Vec3f operator*(const Vec3f& other) const; // componenet wise
	Vec3f& operator*=(const Vec3f& other); // 
	Vec3f operator*(Float_32 s) const; // componenet wise
	Vec3f& operator*=(Float_32 s); // 
	Vec3f operator/(Float_32 s) const;
	Vec3f& operator/=(Float_32 s); 
	Vec3f GetNormalized() const;
	Vec3f& Normalize();
	Float_32 GetMagnitude() const;
	Float_32 GetSqrdMagnitude() const;
};

struct Vec4f
{
	Float_32 x, y, z, w;

	//const, dest, copy
	Vec4f();
	~Vec4f() = default;
	Vec4f(Float_32 _x, Float_32 _y, Float_32 _z, Float_32 _w);
	Vec4f(const Vec4f& other) = default;
	Vec4f& operator=(const Vec4f& other) = default;
	Vec4f(Vec4f&& other) = default;
	Vec4f& operator=(Vec4f&& other) = delete;

	//operations
	Vec4f operator+(const Vec4f& other) const;
	Vec4f operator-(const Vec4f& other) const;
	Vec4f& operator+=(const Vec4f& other);
	Vec4f& operator-=(const Vec4f& other);
	Vec4f operator*(const Vec4f& other) const; // componenet wise
	Vec4f& operator*=(const Vec4f& other); // 
	Vec4f operator*(Float_32 s) const; // componenet wise
	Vec4f& operator*=(Float_32 s); // 
	Vec4f operator/(Float_32 s) const;
	Vec4f& operator/=(Float_32 s);
	Vec4f GetNormalized() const;
	Vec4f& Normalize();
	Float_32 GetMagnitude() const;
	Float_32 GetSqrdMagnitude() const;
};

Float_32 Dot(const Vec3f& a, const Vec3f& b);
Float_32 Dot(const Vec2f& a, const Vec2f& b);
Vec3f Cross(const Vec3f& a, const Vec3f& b);


#endif // !VEC_HPP

#ifdef PRI_VEC_IMPLEMENTATION
#undef PRI_VEC_IMPLEMENTATION

#include <math.h>
#define EPS 0.000001	// very small value

////////////////////////////////////////////////
// Vec2f
////////////////////////////////////////////////
#pragma region Vec2f
	Vec2f::Vec2f()
	{
		x = 0;
		y = 0;
	}

	Vec2f::Vec2f(Float_32 _x, Float_32 _y)
	{
		x = _x;
		y = _y;
	}

	Vec2f Vec2f::operator+(const Vec2f& other) const
	{
		return Vec2f{ x + other.x, y + other.y };
	}
	Vec2f Vec2f::operator-(const Vec2f& other) const
	{
		return Vec2f{ x - other.x, y - other.y };
	}

	Vec2f& Vec2f::operator+=(const Vec2f& other)
	{
		x += other.x;
		y += other.y;
		return *this;
	}

	Vec2f& Vec2f::operator-=(const Vec2f& other)
	{
		x -= other.x;
		y -= other.y;
		return *this;
	}

	Vec2f Vec2f::operator*(const Vec2f& other) const
	{
		return Vec2f{x*other.x, y*other.y};
	}

	Vec2f& Vec2f::operator*=(const Vec2f& other)
	{
		x *= other.x;
		y *= other.y;
		return *this;
	}

	Vec2f Vec2f::operator*(Float_32 s) const
	{
		return Vec2f{ x*s, y*s };
	}

	Vec2f& Vec2f::operator*=(Float_32 s)
	{
		x *= s;
		y *= s;
		return *this;
	}

	Vec2f Vec2f::operator/(Float_32 s) const
	{
		return Vec2f{ x/s, y/s };
	}

	Vec2f& Vec2f::operator/=(Float_32 s)
	{
		x /= s;
		y /= s;
		return *this;
	}

	Vec2f Vec2f::GetNormalized() const
	{
		Float_32 _magnitude = GetMagnitude();
		if (_magnitude < EPS)
			return Vec2f();

		return Vec2f{ x / _magnitude, y / _magnitude };
	}


	Vec2f& Vec2f::Normalize()
	{
		Float_32 _magnitude = GetMagnitude();
		if (_magnitude < EPS)
		{
			x = 0.f;
			y = 0.f;
		}
		else
		{
			x /= _magnitude;
			y /= _magnitude;
		}

		return *this;
	}

	Float_32 Vec2f::GetMagnitude() const
	{
		return sqrtf(x * x + y * y);
	}

	Float_32 Vec2f::GetSqrdMagnitude() const
	{
		return (x * x + y * y);
	}
#pragma endregion

////////////////////////////////////////////////
// Vec3f
////////////////////////////////////////////////

#pragma region Vec3f

	Vec3f::Vec3f()
	{
		x = 0;
		y = 0;
		z = 0;
	}

	Vec3f::Vec3f(Float_32 _x, Float_32 _y, Float_32 _z)
	{
		x = _x;
		y = _y;
		z = _z;
	}

	Vec3f Vec3f::operator+(const Vec3f & other) const
	{
		return Vec3f{ x + other.x, y + other.y, z + other.z };
	}

	Vec3f Vec3f::operator-(const Vec3f & other) const
	{
		return Vec3f{ x - other.x, y - other.y, z - other.z };
	}

	Vec3f& Vec3f::operator+=(const Vec3f & other)
	{
		x += other.x;
		y += other.y;
		z += other.z;
		return *this;
	}

	Vec3f& Vec3f::operator-=(const Vec3f & other)
	{
		x -= other.x;
		y -= other.y;
		z -= other.z;
		return *this;

	}

	Vec3f Vec3f::operator*(const Vec3f & other) const
	{
		return Vec3f{ x * other.x, y * other.y, z * other.z };

	}

	Vec3f & Vec3f::operator*=(const Vec3f & other)
	{
		x *= other.x;
		y *= other.y;
		z *= other.z;
		return *this;
	}

	Vec3f Vec3f::operator*(Float_32 s) const
	{
		return Vec3f{ x*s, y*s, z*s };
	}

	Vec3f& Vec3f::operator*=(Float_32 s)
	{
		x *= s;
		y *= s;
		z *= s;
		return *this;
	}

	Vec3f Vec3f::operator/(Float_32 s) const
	{
		return Vec3f{ x / s, y / s, z / s };
	}

	Vec3f& Vec3f::operator/=(Float_32 s)
	{
		x /= s;
		y /= s;
		z /= s;
		return *this;
	}

	Vec3f Vec3f::GetNormalized() const
	{
		Float_32 _magnitude = GetMagnitude();
		if (_magnitude < EPS)
			return Vec3f();

		return Vec3f{ x / _magnitude, y / _magnitude, z / _magnitude };
	}

	Vec3f & Vec3f::Normalize()
	{
		Float_32 _magnitude = GetMagnitude();
		if (_magnitude < EPS)
		{
			x = 0.f;
			y = 0.f;
			z = 0.f;
		}
		else
		{
			x /= _magnitude;
			y /= _magnitude;
			z /= _magnitude;
		}

		return *this;
	}

	Float_32 Vec3f::GetMagnitude() const
	{
		return sqrtf(x * x + y * y + z * z);
	}

	Float_32 Vec3f::GetSqrdMagnitude() const
	{
		return (x * x + y * y + z * z);
	}
#pragma endregion
////////////////////////////////////////////////
// Vec4f
////////////////////////////////////////////////
#pragma region Vec4f


	Vec4f::Vec4f()
	{
		x = 0;
		y = 0;
		z = 0;
		w = 0;
	}

	Vec4f::Vec4f(Float_32 _x, Float_32 _y, Float_32 _z, Float_32 _w)
	{
		x = _x;
		y = _y;
		z = _z;
		w = _w;
	}

	Vec4f Vec4f::operator+(const Vec4f & other) const
	{
		return Vec4f{ x + other.x, y + other.y, z + other.z, w + other.w};
	}

	Vec4f Vec4f::operator-(const Vec4f & other) const
	{
		return Vec4f{ x - other.x, y - other.y, z - other.z, w - other.w };
	}

	Vec4f& Vec4f::operator+=(const Vec4f & other)
	{
		x += other.x;
		y += other.y;
		z += other.z;
		w += other.w;
		return *this;
	}

	Vec4f& Vec4f::operator-=(const Vec4f & other)
	{
		x -= other.x;
		y -= other.y;
		z -= other.z;
		w -= other.w;
		return *this;

	}

	Vec4f Vec4f::operator*(const Vec4f & other) const
	{
		return Vec4f{ x * other.x, y * other.y, z * other.z, w * other.w };

	}

	Vec4f& Vec4f::operator*=(const Vec4f& other)
	{
		x *= other.x;
		y *= other.y;
		z *= other.z;
		w *= other.w;
		return *this;
	}

	Vec4f Vec4f::operator*(Float_32 s) const
	{
		return Vec4f{ x*s, y*s, z*s, w*w };
	}

	Vec4f& Vec4f::operator*=(Float_32 s)
	{
		x *= s;
		y *= s;
		z *= s;
		w *= s;
		return *this;
	}

	Vec4f Vec4f::operator/(Float_32 s) const
	{
		return Vec4f{ x / s, y / s, z / s, w/ s};
	}

	Vec4f& Vec4f::operator/=(Float_32 s)
	{
		x /= s;
		y /= s;
		z /= s;
		w /= s;
		return *this;
	}

	Vec4f Vec4f::GetNormalized() const
	{
		Float_32 _magnitude = GetMagnitude();
		if (_magnitude < EPS)
			return Vec4f();

		return Vec4f{ x / _magnitude, y / _magnitude, z / _magnitude, w / _magnitude };
	}

	Vec4f& Vec4f::Normalize()
	{
		Float_32 _magnitude = GetMagnitude();
		if (_magnitude < EPS)
		{
			x = 0.f;
			y = 0.f;
			z = 0.f;
			w = 0.f;
		}
		else
		{
			x /= _magnitude;
			y /= _magnitude;
			z /= _magnitude;
			w /= _magnitude;
		}

		return *this;
	}

	Float_32 Vec4f::GetMagnitude() const
	{
		return sqrtf(x * x + y * y + z * z + w * w);
	}

	Float_32 Vec4f::GetSqrdMagnitude() const
	{
		return (x * x + y * y + z * z + w * w);
	}
#pragma endregion
////////////////////////////////////////////////
// Static functions
////////////////////////////////////////////////
#pragma region Statics
	// static functions
	Float_32 Dot(const Vec3f& a, const Vec3f& b)
	{
		return (a.x*b.x + a.y * b.y + a.z * b.z);
	}
	Float_32 Dot(const Vec2f& a, const Vec2f& b)
	{
		return (a.x*b.x + a.y * b.y);
	}
	Vec3f Cross(const Vec3f& a, const Vec3f& b)
	{
		return Vec3f{
			a.y*b.z - a.z*b.y,
			a.z*b.x - a.x*b.z,
			a.x*b.y - a.y*b.x
		};
	}
#pragma endregion

#endif
