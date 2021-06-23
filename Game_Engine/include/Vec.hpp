#ifndef PRI_VEC_HPP
#define PRI_VEC_HPP

#define PI 3.14159265

// may need to redef depending on compiler
typedef int Int_32;
typedef unsigned int Uint_32;
typedef float Float_32;
///////////////////////////////////

struct Vec2f;
struct Vec3f;
struct Vec4f;
struct Mat3x3;
struct Mat4x4;

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
	Vec2f& operator=(Vec2f&& other) = default;

	//operations
	Vec2f operator+(const Vec2f& other) const;
	Vec2f operator-(const Vec2f& other) const;
	Vec2f operator-() const;
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
	operator Vec3f() const;
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
	Vec3f& operator=(Vec3f&& other) = default;

	//operations
	Vec3f operator+(const Vec3f& other) const;
	Vec3f operator-(const Vec3f& other) const;
	Vec3f operator-() const;
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
	operator Vec2f() const;

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
	Vec4f& operator=(Vec4f&& other) = default;

	//operations
	Vec4f operator+(const Vec4f& other) const;
	Vec4f operator-(const Vec4f& other) const;
	Vec4f operator-() const;
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
	operator Vec3f() const;
	operator Vec2f() const;

};

struct Point3d : Vec3f
{
	Point3d() = default;
	Point3d(Float_32 _x, Float_32 _y, Float_32 _z);
	Point3d operator+(const Vec3f& other) const;
	Vec3f operator-(const Point3d& other) const;
	Point3d operator-(const Vec3f& other) const;

};

struct Mat4x4
{
	Float_32 data[4*4];
	Mat4x4();
	Mat4x4(Vec4f _c0, Vec4f _c1, Vec4f _c2, Vec4f _c3);
	Mat4x4(	Float_32 v00, Float_32 v01, Float_32 v02, Float_32 v03,
			Float_32 v10, Float_32 v11, Float_32 v12, Float_32 v13, 
			Float_32 v20, Float_32 v21, Float_32 v22, Float_32 v23, 
			Float_32 v30, Float_32 v31, Float_32 v32, Float_32 v33);
	~Mat4x4() = default;

	Mat4x4 operator+(const Mat4x4& m) const;
	Mat4x4 operator-(const Mat4x4& m) const;
	Mat4x4 operator*(const Mat4x4& m) const;
	Vec4f operator*(const Vec4f& vec4) const;
	Vec4f operator*(const Vec3f& v) const;
	Point3d operator*(const Point3d& v) const;
	Vec4f operator*(const Vec2f& v) const;
	Mat4x4 operator/(Float_32 v) const;
	Mat4x4 operator*(Float_32 v) const;
	//Mat4x4 Transpose();
	//Mat4x4 GetTranspose() const;
	//Mat4x4 Invert();
	Mat4x4 GetInverse() const;
	Mat4x4 GetTransformInverse() const;
	//Float_32 GetDeterminant() const;
	const Vec4f& operator[](Uint_32 col) const;
	Float_32 operator()(Uint_32 row, Uint_32 col) const;
	Float_32& operator()(Uint_32 row, Uint_32 col);

	static Mat4x4 Orthogrpahic(Float_32 _left, Float_32 _right, Float_32 _bottom, Float_32 _top, Float_32 _near, Float_32 _far);
	static Mat4x4 Identity();


};

struct Quaternion
{
	Float_32 x, y, z, w;

	Quaternion() = default;
	Quaternion(Float_32 _x, Float_32 _y, Float_32 _z, Float_32 _w);
	Quaternion(Vec3f v, Float_32 s);
	const Vec3f& GetVectorPart() const;
	Mat4x4 GetRotationMatrix();
	void SetRotationMatrix(const Mat4x4& m);
	Quaternion operator*(const Quaternion& q) const;
	Quaternion& Normalize();
	static Quaternion QuaternionFromAngleAxis(const Vec3f& a, Float_32 th);
	static Quaternion EulerToQuaternion(const Vec3f& euler);
	static Vec3f QuaternionToEuler(const Quaternion& q);
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

	Vec2f Vec2f::operator-() const
	{
		return Vec2f{ -x,-y };
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

	Vec2f::operator Vec3f() const
	{
		return Vec3f{ x, y, 0 };
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

	Vec3f Vec3f::operator-() const
	{
		return Vec3f{ -x,-y,-z };
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

	Vec3f::operator Vec2f() const
	{
		//return *((Vec2f*)this);
		return Vec2f{ x, y };
	}
#pragma endregion

Point3d::Point3d(Float_32 _x, Float_32 _y, Float_32 _z) :Vec3f{ _x, _y, _z } {};

Point3d Point3d::operator+(const Vec3f& other) const
{
	return Point3d{ x + other.x, y + other.y, z + other.z };
}

Vec3f Point3d::operator-(const Point3d& other) const
{
	return Vec3f{ x + other.x, y + other.y, z + other.z };
}

Point3d Point3d::operator-(const Vec3f& other) const
{
	return Point3d{ x + other.x, y + other.y, z + other.z };
}

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

	Vec4f Vec4f::operator-() const
	{
		return Vec4f{ -x,-y,-z,-w };
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

	Vec4f::operator Vec3f() const
	{
		//return *((Vec2f*)this);
		return Vec3f{ x, y, z};
	}

	Vec4f::operator Vec2f() const
	{
		//return *((Vec2f*)this);
		return Vec2f{ x, y};
	}

#pragma endregion


	Quaternion::Quaternion(Float_32 _x, Float_32 _y, Float_32 _z, Float_32 _w)
	{
		x = _x;
		y = _y;
		z = _z;
		w = _w;
	}

	Quaternion::Quaternion(Vec3f v, Float_32 s)
	{
		x = v.x;
		y = v.y;
		z = v.z;
		w = s;
	}

	const Vec3f& Quaternion::GetVectorPart() const
	{
		return reinterpret_cast<const Vec3f&>(x);
	}

	Mat4x4 Quaternion::GetRotationMatrix()
	{
		Float_32 x2 = x * x;
		Float_32 y2 = y * y;
		Float_32 z2 = z * z;
		Float_32 xy = x * y;
		Float_32 xz = x * z;
		Float_32 yz = y * z;
		Float_32 wx = w * x;
		Float_32 wy = w * y;
		Float_32 wz = w * z;

		return Mat4x4{
			1.0f - 2.0f * (y2 + z2),	2.0f * (xy - wz),	2.0f*(xz + wy),			0,
			2.0f*(xy+wz),				1.0f-2.0f*(x2+z2),	2.0f*(yz-wx),			0,
			2.0f*(xz-wy),				2.0f*(yz+wx),		1.0f-2.0f*(x2+y2),		0,
			0,							0,					0,						1
		};
	}

	void Quaternion::SetRotationMatrix(const Mat4x4& m)
	{
	}
	Quaternion Quaternion::operator*(const Quaternion& q) const
	{
		const Quaternion& q1 = (*this);
		return Quaternion{
			q1.w * q.x + q1.x*q.w + q1.y * q.z - q1.z*q.y,
			q1.w * q.y - q1.x*q.z + q1.y * q.w + q1.z*q.x,
			q1.w * q.z + q1.x*q.y - q1.y * q.x + q1.z*q.w,
			q1.w * q.w - q1.x*q.x - q1.y * q.y - q1.z*q.z
		};
	}
	Quaternion Quaternion::QuaternionFromAngleAxis(const Vec3f& a, Float_32 th)
	{
		Float_32 s = sinf(th/2);
		Float_32 c = cosf(th/2);

		return Quaternion(a.x*s, a.y*s, a.z*s, c);
	}

	Quaternion Quaternion::EulerToQuaternion(const Vec3f& euler)
	{
		/*Float_32 cr = cosf(euler.z * 0.5);
		Float_32 sr = sinf(euler.z * 0.5);
		Float_32 cp = cosf(euler.x * 0.5);
		Float_32 sp = sinf(euler.x * 0.5);
		Float_32 cy = cosf(euler.y * 0.5);
		Float_32 sy = sinf(euler.y * 0.5);
		
 		return Quaternion{
			cr * cp * cy + sr * sp * sy,
			sr * cp * cy - cr * sp * sy,
			cr * sp * cy + sr * cp * sy,
			cr * cp * sy - sr * sp * cy
		};*/


		Quaternion qx = Quaternion::QuaternionFromAngleAxis(Vec3f{ 1,0,0 }, euler.x);
		Quaternion qy = Quaternion::QuaternionFromAngleAxis(Vec3f{ 0,1,0 }, euler.y);
		Quaternion qz = Quaternion::QuaternionFromAngleAxis(Vec3f{ 0,0,1 }, euler.z);

		return (qz*(qy*qx));
	}

	Vec3f Quaternion::QuaternionToEuler(const Quaternion& q)
	{
		return Vec3f();
	}

	Quaternion& Quaternion::Normalize()
	{
		Float_32 _magnitude = sqrtf( x * x + y * y + z * z + w * w);
		if (_magnitude < EPS)
		{
			x = 0;
			y = 0;
			z = 0;
			w = 0;
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


////////////////////////////////////////////////
// Mat4x4
////////////////////////////////////////////////
	
	Mat4x4::Mat4x4()
	{
		Mat4x4& _m = (*this);
		_m(0, 0) = 0; _m(0, 1) = 0; _m(0, 2) = 0; _m(0, 3) = 0;
		_m(1, 0) = 0; _m(1, 1) = 0; _m(1, 2) = 0; _m(1, 3) = 0;
		_m(2, 0) = 0; _m(2, 1) = 0; _m(2, 2) = 0; _m(2, 3) = 0;
		_m(3, 0) = 0; _m(3, 1) = 0; _m(3, 2) = 0; _m(3, 3) = 0;
	}

	Mat4x4::Mat4x4(Vec4f _c0, Vec4f _c1, Vec4f _c2, Vec4f _c3)
	{
		Mat4x4& _m = (*this);
		_m(0, 0) = _c0.x; _m(0, 1) = _c1.x; _m(0, 2) = _c2.x; _m(0, 3) = _c3.x;
		_m(1, 0) = _c0.y; _m(1, 1) = _c1.y; _m(1, 2) = _c2.y; _m(1, 3) = _c3.y;
		_m(2, 0) = _c0.z; _m(2, 1) = _c1.z; _m(2, 2) = _c2.z; _m(2, 3) = _c3.z;
		_m(3, 0) = _c0.w; _m(3, 1) = _c1.w; _m(3, 2) = _c2.w; _m(3, 3) = _c3.w;
	}

	Mat4x4::Mat4x4(	Float_32 v00, Float_32 v01, Float_32 v02, Float_32 v03,
					Float_32 v10, Float_32 v11, Float_32 v12, Float_32 v13,
					Float_32 v20, Float_32 v21, Float_32 v22, Float_32 v23,
					Float_32 v30, Float_32 v31, Float_32 v32, Float_32 v33)
	{
		Mat4x4& _m = (*this);
		_m(0, 0) = v00; _m(0, 1) = v01; _m(0, 2) = v02; _m(0, 3) = v03;
		_m(1, 0) = v10; _m(1, 1) = v11; _m(1, 2) = v12; _m(1, 3) = v13;
		_m(2, 0) = v20; _m(2, 1) = v21; _m(2, 2) = v22; _m(2, 3) = v23;
		_m(3, 0) = v30; _m(3, 1) = v31; _m(3, 2) = v32; _m(3, 3) = v33;
	}

	Mat4x4 Mat4x4::operator+(const Mat4x4& m) const
	{
		const Mat4x4& _m = (*this);
		return Mat4x4{
			_m(0,0) + m(0,0), _m(0,1) + m(0,1), _m(0,2) + m(0,2), _m(0,3) + m(0,3),
			_m(1,0) + m(1,0), _m(1,1) + m(1,1), _m(1,2) + m(1,2), _m(1,3) + m(1,3),
			_m(2,0) + m(2,0), _m(2,1) + m(2,1), _m(2,2) + m(2,2), _m(2,3) + m(2,3),
			_m(3,0) + m(3,0), _m(3,1) + m(3,1), _m(3,2) + m(3,2), _m(3,3) + m(3,3)
		};
	}

	Mat4x4 Mat4x4::operator-(const Mat4x4& m) const
	{
		const Mat4x4& _m = (*this);
		return Mat4x4{
			_m(0,0) - m(0,0), _m(0,1) - m(0,1), _m(0,2) - m(0,2), _m(0,3) - m(0,3),
			_m(1,0) - m(1,0), _m(1,1) - m(1,1), _m(1,2) - m(1,2), _m(1,3) - m(1,3),
			_m(2,0) - m(2,0), _m(2,1) - m(2,1), _m(2,2) - m(2,2), _m(2,3) - m(2,3),
			_m(3,0) - m(3,0), _m(3,1) - m(3,1), _m(3,2) - m(3,2), _m(3,3) - m(3,3)
		};
	}

	Mat4x4 Mat4x4::operator*(const Mat4x4& m) const
	{
		const Mat4x4& _m = (*this);

		return Mat4x4(
			_m(0,0)*m(0,0) + _m(0,1)*m(1,0) + _m(0,2)*m(2,0) + _m(0,3)*m(3,0),
			_m(0,0)*m(0,1) + _m(0,1)*m(1,1) + _m(0,2)*m(2,1) + _m(0,3)*m(3,1),
			_m(0,0)*m(0,2) + _m(0,1)*m(1,2) + _m(0,2)*m(2,2) + _m(0,3)*m(3,2),
			_m(0,0)*m(0,3) + _m(0,1)*m(1,3) + _m(0,2)*m(2,3) + _m(0,3)*m(3,3),

			_m(1,0)*m(0,0) + _m(1,1)*m(1,0) + _m(1,2)*m(2,0) + _m(1,3)*m(3,0),
			_m(1,0)*m(0,1) + _m(1,1)*m(1,1) + _m(1,2)*m(2,1) + _m(1,3)*m(3,1),
			_m(1,0)*m(0,2) + _m(1,1)*m(1,2) + _m(1,2)*m(2,2) + _m(1,3)*m(3,2),
			_m(1,0)*m(0,3) + _m(1,1)*m(1,3) + _m(1,2)*m(2,3) + _m(1,3)*m(3,3),

			_m(2,0)*m(0,0) + _m(2,1)*m(1,0) + _m(2,2)*m(2,0) + _m(2,3)*m(3,0),
			_m(2,0)*m(0,1) + _m(2,1)*m(1,1) + _m(2,2)*m(2,1) + _m(2,3)*m(3,1),
			_m(2,0)*m(0,2) + _m(2,1)*m(1,2) + _m(2,2)*m(2,2) + _m(2,3)*m(3,2),
			_m(2,0)*m(0,3) + _m(2,1)*m(1,3) + _m(2,2)*m(2,3) + _m(2,3)*m(3,3),

			_m(3,0)*m(0,0) + _m(3,1)*m(1,0) + _m(3,2)*m(2,0) + _m(3,3)*m(3,0),
			_m(3,0)*m(0,1) + _m(3,1)*m(1,1) + _m(3,2)*m(2,1) + _m(3,3)*m(3,1),
			_m(3,0)*m(0,2) + _m(3,1)*m(1,2) + _m(3,2)*m(2,2) + _m(3,3)*m(3,2),
			_m(3,0)*m(0,3) + _m(3,1)*m(1,3) + _m(3,2)*m(2,3) + _m(3,3)*m(3,3)
		);
	}

	Vec4f Mat4x4::operator*(const Vec4f& vec4) const
	{
		const Mat4x4& _m = (*this);
		Vec4f r;
		r.x = (_m(0, 0) * vec4.x) + (_m(0, 1) * vec4.y) + (_m(0, 2) * vec4.z) + (_m(0, 3) * vec4.w);
		r.y = (_m(1, 0) * vec4.x) + (_m(1, 1) * vec4.y) + (_m(1, 2) * vec4.z) + (_m(1, 3) * vec4.w);
		r.z = (_m(2, 0) * vec4.x) + (_m(2, 1) * vec4.y) + (_m(2, 2) * vec4.z) + (_m(2, 3) * vec4.w);
		r.w = (_m(3, 0) * vec4.x) + (_m(3, 1) * vec4.y) + (_m(3, 2) * vec4.z) + (_m(3, 3) * vec4.w);
		return r;
	}

	Vec4f Mat4x4::operator*(const Vec3f& vec3) const
	{
		const Mat4x4& _m = (*this);
		Vec4f r;
		r.x = (_m(0, 0) * vec3.x) + (_m(0, 1) * vec3.y) + (_m(0, 2) * vec3.z) + (_m(0, 3) * 1);
		r.y = (_m(1, 0) * vec3.x) + (_m(1, 1) * vec3.y) + (_m(1, 2) * vec3.z) + (_m(1, 3) * 1);
		r.z = (_m(2, 0) * vec3.x) + (_m(2, 1) * vec3.y) + (_m(2, 2) * vec3.z) + (_m(2, 3) * 1);
		r.w = (_m(3, 0) * vec3.x) + (_m(3, 1) * vec3.y) + (_m(3, 2) * vec3.z) + (_m(3, 3) * 1);
		return r;
	}

	Point3d Mat4x4::operator*(const Point3d& p3) const
	{
		const Mat4x4& _m = (*this);
		Point3d r;
		r.x = (_m(0, 0) * p3.x) + (_m(0, 1) * p3.y) + (_m(0, 2) * p3.z) + (_m(0, 3) * 1);
		r.y = (_m(1, 0) * p3.x) + (_m(1, 1) * p3.y) + (_m(1, 2) * p3.z) + (_m(1, 3) * 1);
		r.z = (_m(2, 0) * p3.x) + (_m(2, 1) * p3.y) + (_m(2, 2) * p3.z) + (_m(2, 3) * 1);
		return r;
	}

	Vec4f Mat4x4::operator*(const Vec2f& vec2) const
	{
		const Mat4x4& _m = (*this);
		Vec4f r;
		r.x = (_m(0, 0) * vec2.x) + (_m(0, 1) * vec2.y) + _m(0, 2)	+ _m(0, 3);
		r.y = (_m(1, 0) * vec2.x) + (_m(1, 1) * vec2.y) + _m(1, 2) + _m(1, 3);
		r.z = (_m(2, 0) * vec2.x) + (_m(2, 1) * vec2.y) + _m(2, 2) + _m(2, 3);
		r.w = (_m(3, 0) * vec2.x) + (_m(3, 1) * vec2.y) + _m(3, 2 ) + _m(3, 3);
		return r;
	}

	//Mat4x4 Mat4x4::operator^(Mat4x4 m) const
	//{
	//	Matrix4x4<T> r;
	//	for (Int_32 i = 0; i < 4; i++)
	//	{
	//		for (Int_32 j = 0; j < 4; j++)
	//		{
	//			r.data[(i * 4) + j] = data[(i * 4) + j] * m.data[(i * 4) + j];
	//		}
	//	}
	//	return r;
	//}

	Mat4x4 Mat4x4::operator/(Float_32 s) const
	{
		const Mat4x4& _m = (*this);
		return Mat4x4{
			_m(0, 0) / s, _m(0, 1) / s, _m(0, 2) / s, _m(0, 3) / s,
			_m(1, 0) / s, _m(1, 1) / s, _m(1, 2) / s, _m(1, 3) / s,
			_m(2, 0) / s, _m(2, 1) / s, _m(2, 2) / s, _m(2, 3) / s,
			_m(3, 0) / s, _m(3, 1) / s, _m(3, 2) / s, _m(3, 3) / s
		};
	}

	Mat4x4 Mat4x4::operator*(Float_32 s) const
	{
		const Mat4x4& _m = (*this);
		return Mat4x4{
			_m(0, 0) * s, _m(0, 1) * s, _m(0, 2) * s, _m(0, 3) * s,
			_m(1, 0) * s, _m(1, 1) * s, _m(1, 2) * s, _m(1, 3) * s,
			_m(2, 0) * s, _m(2, 1) * s, _m(2, 2) * s, _m(2, 3) * s,
			_m(3, 0) * s, _m(3, 1) * s, _m(3, 2) * s, _m(3, 3) * s
		};
	}

	// every 4 elements is a column
	Float_32 Mat4x4::operator()(Uint_32 row, Uint_32 col) const
	{
		return data[col * 4 + row];
	}

	Float_32& Mat4x4::operator()(Uint_32 row, Uint_32 col)
	{
		return data[col * 4 + row];

	}

	const Vec4f& Mat4x4::operator[](Uint_32 col) const
	{
		return *(reinterpret_cast<const Vec4f*>(&data[col * 4]));
	}

	/*Mat4x4 Mat4x4::GetTranspose() const
	{
		return Mat4x4();
	}
*/
	Mat4x4 Mat4x4::GetInverse() const
	{
		const Mat4x4& _m = (*this);
		const Vec3f& a = reinterpret_cast<const Vec3f&>(_m[0]);
		const Vec3f& b = reinterpret_cast<const Vec3f&>(_m[1]);
		const Vec3f& c = reinterpret_cast<const Vec3f&>(_m[2]);
		const Vec3f& d = reinterpret_cast<const Vec3f&>(_m[3]);

		Float_32 x = _m(3, 0);
		Float_32 y = _m(3, 1);
		Float_32 z = _m(3, 2);
		Float_32 w = _m(3, 3);

		Vec3f s = Cross(a, b);
		Vec3f t = Cross(c, d);
		Vec3f u = a * y - b * x;
		Vec3f v = c * w - d * z;

		Float_32 inv_det = (1.0 / (Dot(s, v) + Dot(t, u)));
		s *= inv_det;
		t *= inv_det;
		u *= inv_det;
		v *= inv_det;

		Vec3f r0 = Cross(b, v) + t * y;
		Vec3f r1 = Cross(v, a) - t * x;
		Vec3f r2 = Cross(d, u) + s * w;
		Vec3f r3 = Cross(u, c) - s * z;

		return (Mat4x4{
			r0.x, r0.y, r0.z, -Dot(b,t),
			r1.x, r1.y, r1.z,  Dot(a,t),
			r2.x, r2.y, r2.z, -Dot(d,s),
			r3.x, r3.y, r3.z,  Dot(c,s)
			});
	}

	Mat4x4 Mat4x4::GetTransformInverse() const
	{
		const Mat4x4& _m = (*this);
		const Vec3f& a = reinterpret_cast<const Vec3f&>(_m[0]);
		const Vec3f& b = reinterpret_cast<const Vec3f&>(_m[1]);
		const Vec3f& c = reinterpret_cast<const Vec3f&>(_m[2]);
		const Vec3f& d = reinterpret_cast<const Vec3f&>(_m[3]);

		Vec3f s = Cross(a, b);
		Vec3f t = Cross(c, d);

		Float_32 inv_det = (1.0 / Dot(s, c));
		s *= inv_det;
		t *= inv_det;
		Vec3f v = c * inv_det;
		
		Vec3f r0 = Cross(b, v);
		Vec3f r1 = Cross(v, a);


		return (Mat4x4{
			r0.x, r0.y, r0.z, -Dot(b,t),
			r1.x, r1.y, r1.z,  Dot(a,t),
			s.x, s.y, s.z, -Dot(d,s),
			0,0,0,1
			});
	}

	Mat4x4 Mat4x4::Identity()
	{
		return Mat4x4{	1,0,0,0,
						0,1,0,0,
						0,0,1,0,
						0,0,0,1 };
		
	}

	Mat4x4 Mat4x4::Orthogrpahic(Float_32 _left, Float_32 _right, Float_32 _bottom, Float_32 _top, Float_32 _near, Float_32 _far)
	{
		
		return Mat4x4{	2.f / (_right - _left),		0,	0,	-(_right + _left) / (_right - _left),
						0,	2.f / (_top - _bottom),	0, -(_top + _bottom) / (_top - _bottom),
						0,	0,	-2.f / (_far - _near),	-(_far + _near) / (_far - _near),
						0,	0,	0,	1 };
	}



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
