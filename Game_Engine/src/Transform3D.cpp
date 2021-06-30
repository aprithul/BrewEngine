#include "Transform3D.hpp"
namespace  PrEngine
{
    Transform3D::Transform3D():Component(COMP_TRANSFORM_3D)
    {
		transformation = Mat4x4::Identity();
		//rotation_transformation = Mat4x4::Identity();
        
		position = Point3d(0,0,0);
        scale = Vec3f(1,1,1);
        rotation = Vec3f(0,0,0);

		parent_transform = 0;
    }

    Transform3D::~Transform3D()
    {

    }


    void Transform3D::update_transformation()
    {
		Float_32 a = rotation.x * PI / 180.f;
		Float_32 b = rotation.y * PI / 180.f;
		Float_32 c = rotation.z * PI / 180.f;

		Mat4x4 scale_m = Mat4x4::Identity();
		scale_m(0, 0) = scale.x;
		scale_m(1, 1) = scale.y;
		scale_m(2, 2) = scale.z;

		Mat4x4 translation = Mat4x4::Identity();
		translation(0, 3) = position.x;
		translation(1, 3) = position.y;
		translation(2, 3) = position.z;

		transformation = transforms[parent_transform].transformation * translation * Quaternion::EulerToQuaternion(Vec3f{a,b,c}).GetRotationMatrix()* scale_m;
	}

	void Transform3D::update()
	{
		update_transformation();
	}

    const Vec3f Transform3D::get_forward()
    {
        return transformation * Vec3f(0, 0, 1.f);
    }

    const Vec3f Transform3D::get_right()
    {
        return transformation * Vec3f(1.f, 0, 0.f);
    }

    const Vec3f Transform3D::get_up()
    {
        return transformation * Vec3f(0, 1.f, 0.f);
    }

	void Transform3D::Translate(const Vec3f& translation)
	{
		position += translation;
	}

	void Transform3D::Rotate(Float_32 _x, Float_32 _y, Float_32 _z)
	{
		rotation.x += _x;
		rotation.y += _y;
		rotation.z += _z;
		is_dirty = true;
	}

	void Transform3D::Scale(Float_32 _x, Float_32 _y, Float_32 _z)
	{
		scale.x += _x;
		scale.y += _y;
		scale.z += _z;
		is_dirty = true;
	}

	void Transform3D::set_position(Float_32 _x, Float_32 _y, Float_32 _z)
	{
		position = {_x, _y, _z};
		is_dirty = true;
	}

	void Transform3D::set_position(Point3d _position)
	{
		position = _position;
		is_dirty = true;
	}

	void Transform3D::set_rotation(Float_32 _x, Float_32 _y, Float_32 _z)
	{
		rotation = { _x, _y, _z };
		is_dirty = true;
	}

	void Transform3D::set_rotation(Vec3f _rotation)
	{
		rotation = _rotation;
		is_dirty = true;
	}

	void Transform3D::set_scale(Float_32 _x, Float_32 _y, Float_32 _z)
	{
		scale = { _x, _y, _z };
		is_dirty = true;
	}

	void Transform3D::set_scale(Vec3f _scale)
	{
		scale = _scale;
		is_dirty = true;
	}

	Point3d Transform3D::get_position()
	{
		return position;
	}
	Vec3f Transform3D::get_rotation()
	{
		return rotation;
	}
	Vec3f Transform3D::get_scale()
	{
		return scale;
	}


	
    std::string Transform3D::to_string()
    {
    	std::string text = std::to_string(COMP_TRANSFORM_3D)+",";

		Vec3f _position = get_global_position();
		Vec3f _rotation = get_global_rotation();

		text += std::to_string(_position.x) + "," + std::to_string(_position.y) + "," + std::to_string(_position.z) + ",";
		text += std::to_string(scale.x) + "," + std::to_string(scale.y) + "," + std::to_string(scale.z) + ",";
		text += std::to_string(_rotation.x) + "," + std::to_string(_rotation.y) + "," + std::to_string(_rotation.z) + ",";
		text += std::to_string(parent_transform);

		return text;
    }


	Point3d Transform3D::get_global_position()
	{
		//Vec3f parent_pos;
		Mat4x4 parent_t = Mat4x4::Identity();
		if (parent_transform)
			parent_t = transforms[parent_transform].transformation;
		auto pos = parent_t * position;
		return pos;
	}

	Vec3f Transform3D::get_global_scale()
	{
		//Vec3f parent_pos;
		Mat4x4 parent_t = Mat4x4::Identity();
		if (parent_transform)
			parent_t = transforms[parent_transform].transformation;
		auto _scale = parent_t * scale;
		return _scale;
	}

	Vec3f Transform3D::get_global_rotation()
	{
		Vec3f parent_rotation;
		if (parent_transform)
			parent_rotation = transforms[parent_transform].get_global_rotation();

		return parent_rotation + rotation;
	}

	Point3d Transform3D::get_global_to_local_position(Point3d global_pos)
	{
		Mat4x4 parent_t = Mat4x4::Identity();
		Point3d parent_gp;
		if (parent_transform)
		{
			parent_gp = transforms[parent_transform].get_global_position();
			parent_t = transforms[parent_transform].transformation;
		}

		Point3d local_pos = Point3d() + parent_t.GetTransformInverse() * (global_pos - parent_gp);
		return local_pos;
	}

	Vec3f Transform3D::get_global_to_local_rotation(Vec3f global_rot)
	{
		Vec3f parent_rot;
		if (parent_transform)
			parent_rot = transforms[parent_transform].get_global_rotation();

		Vec3f local_rot = global_rot - parent_rot;
		return local_rot;
	}
}
