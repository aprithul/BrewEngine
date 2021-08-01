#include "Transform3D.hpp"
#include "EntityManagementSystemModule.hpp"
namespace  PrEngine
{
	Transform3D::Transform3D() :Component(COMP_TRANSFORM_3D)
	{
		initialize();
	}

	Transform3D::~Transform3D()
	{

	}

	void Transform3D::initialize()
	{
		transformation = Mat4x4::Identity();

		local_position = Point3d(0, 0, 0);
		local_scale = Vec3f(1, 1, 1);
		local_rotation = Quaternion::GetIdentity();

		parent_transform = 0;
		is_dirty = true;
	}

	void Transform3D::start()
	{

	}

	void Transform3D::end()
	{

	}

	void Transform3D::update_transformation(Bool_8 is_parent_dirty)
	{
		is_dirty |= is_parent_dirty;

		if (is_dirty)
		{
/*
			Float_32 a = local_rotation.x * PI / 180.f;
			Float_32 b = local_rotation.y * PI / 180.f;
			Float_32 c = local_rotation.z * PI / 180.f;
*/
			Mat3x3 scale_m = Mat3x3::Identity();
			scale_m(0, 0) = local_scale.x;
			scale_m(1, 1) = local_scale.y;
			scale_m(2, 2) = local_scale.z;

			Mat4x4 translation = Mat4x4::Identity();
			translation(0, 3) = local_position.x;
			translation(1, 3) = local_position.y;
			translation(2, 3) = local_position.z;

			transformation = transform_system.get_component(parent_transform).transformation * translation * local_rotation.GetRotationMatrix() * scale_m;
		}

		for (Uint_32 child : *children)
		{
			if (child)
				transform_system.get_component(child).update_transformation(is_dirty);
		}

		is_dirty = false;
	}

	void Transform3D::update(Float_32 dt)
	{
		// only gets called for top level transforms, so parent won't be dirty
		update_transformation(false);
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
		set_global_position(get_global_position() + translation);
//		local_position += translation;
		//is_dirty = true;
	}

	void Transform3D::Rotate(Float_32 _x, Float_32 _y, Float_32 _z)
	{
		local_rotation = Quaternion::EulerToQuaternion({ _x, _y, _z }) * local_rotation;
		local_rotation.Normalize();
		//local_rotation.x += _x;
		//local_rotation.y += _y;
		//local_rotation.z += _z;
		is_dirty = true;
	}

	void Transform3D::Scale(Float_32 _x, Float_32 _y, Float_32 _z)
	{
		local_scale.x += _x;
		local_scale.y += _y;
		local_scale.z += _z;
		is_dirty = true;
	}

	void Transform3D::set_local_position(Float_32 _x, Float_32 _y, Float_32 _z)
	{
		local_position = { _x, _y, _z };
		is_dirty = true;
	}

	void Transform3D::set_local_position(Point3d _position)
	{
		local_position = _position;
		is_dirty = true;
	}

	void Transform3D::set_local_rotation(Float_32 _x, Float_32 _y, Float_32 _z)
	{
		local_rotation = Quaternion::EulerToQuaternion({ _x, _y, _z });
		is_dirty = true;
	}

	void Transform3D::set_local_rotation(Quaternion _rotation)
	{
		local_rotation = _rotation;
		is_dirty = true;
	}

	void Transform3D::set_local_rotation(Vec3f _rotation)
	{
		local_rotation = Quaternion::EulerToQuaternion(_rotation);
		is_dirty = true;
	}

	void Transform3D::set_local_scale(Float_32 _x, Float_32 _y, Float_32 _z)
	{
		local_scale = { _x, _y, _z };
		is_dirty = true;
	}

	void Transform3D::set_local_scale(Vec3f _scale)
	{
		local_scale = _scale;
		is_dirty = true;
	}

	void Transform3D::set_global_position(Float_32 _x, Float_32 _y, Float_32 _z)
	{
		Point3d new_glob_pos = { _x, _y, _z };
		local_position = transform_system.get_component(parent_transform).transformation.GetInverse() * new_glob_pos;
		is_dirty = true;
	}
	void Transform3D::set_global_position(Point3d _position)
	{
		local_position = transform_system.get_component(parent_transform).transformation.GetInverse() * _position;
		is_dirty = true;
	}
	void Transform3D::set_global_rotation(Float_32 _x, Float_32 _y, Float_32 _z)
	{
		Mat3x3 new_glob_rot = Quaternion::EulerToQuaternion({ _x, _y, _z }).GetRotationMatrix();
		local_rotation.SetRotationMatrix(transform_system.get_component(parent_transform).transformation.GetInverse() * new_glob_rot);
		local_rotation.Normalize();
		is_dirty = true;
	}
	void Transform3D::set_global_rotation(Quaternion _rotation)
	{
		Mat4x4 _inv = transform_system.get_component(parent_transform).transformation.GetInverse();
		Mat3x3 _rot = _rotation.GetRotationMatrix();
		local_rotation.SetRotationMatrix( _inv * _rot);
		is_dirty = true;

	}
	void Transform3D::set_global_scale(Float_32 _x, Float_32 _y, Float_32 _z)
	{
		Vec3f new_glob_scl = { _x, _y, _z };
		local_scale = transform_system.get_component(parent_transform).transformation.GetInverse() * new_glob_scl;
		is_dirty = true;

	}
	void Transform3D::set_global_scale(Vec3f _scale)
	{
		local_scale = transform_system.get_component(parent_transform).transformation.GetInverse() * _scale;
		is_dirty = true;
	}


	Point3d Transform3D::get_local_position()
	{
		return local_position;
	}
	const Quaternion& Transform3D::get_local_rotation()
	{
		return local_rotation;
	}
	Vec3f Transform3D::get_local_scale()
	{
		return local_scale;
	}


	
    std::string Transform3D::to_string()
    {
    	std::string text = std::to_string(COMP_TRANSFORM_3D)+",";

		Vec3f _position = get_global_position();
		//Vec3f _rotation = Quaternion::QuaternionToEuler();

		text += std::to_string(_position.x) + "," + std::to_string(_position.y) + "," + std::to_string(_position.z) + ",";
		text += std::to_string(local_scale.x) + "," + std::to_string(local_scale.y) + "," + std::to_string(local_scale.z) + ",";
		text += std::to_string(local_rotation.x) + "," + std::to_string(local_rotation.y) + "," + std::to_string(local_rotation.z) + "," + std::to_string(local_rotation.w) + ",";
		text += std::to_string(parent_transform);

		return text;
    }


	Point3d Transform3D::get_global_position() const 
	{
		//Vec3f parent_pos;

		auto pos = transform_system.get_component(parent_transform).transformation * local_position;
		return pos;
	}

	Vec3f Transform3D::get_global_scale()
	{
		//Vec3f parent_pos;
		auto _scale = transform_system.get_component(parent_transform).transformation * local_scale;
		return _scale;
	}

	Quaternion Transform3D::get_global_rotation()
	{
		Quaternion _rotation = Quaternion::GetIdentity();
		_rotation.SetRotationMatrix(transform_system.get_component(parent_transform).transformation * local_rotation.GetRotationMatrix());
		_rotation.Normalize();
		return _rotation;
	}

	//Point3d Transform3D::get_global_to_local_position(Point3d global_pos)
	//{
	//	Mat4x4 parent_t = Mat4x4::Identity();
	//	Point3d parent_gp;
	//	if (parent_transform)
	//	{
	//		parent_gp = transforms[parent_transform].get_global_position();
	//		parent_t = transforms[parent_transform].transformation;
	//	}

	//	Point3d local_pos = Point3d() + parent_t.GetTransformInverse() * (global_pos - parent_gp);
	//	return local_pos;
	//}

	//Vec3f Transform3D::get_global_to_local_rotation(Vec3f global_rot)
	//{
	//	Vec3f parent_rot;
	//	if (parent_transform)
	//		parent_rot = transforms[parent_transform].get_global_rotation();

	//	Vec3f local_rot = global_rot - parent_rot;
	//	return local_rot;
	//}
}
