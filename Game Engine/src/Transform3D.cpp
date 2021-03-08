#include "Transform3D.hpp"
namespace  PrEngine
{
    Transform3D::Transform3D():Component(COMP_TRANSFORM_3D)
    {
        transformation = Matrix4x4<Float_32>::identity();
        rotation_transformation = Matrix4x4<Float_32>::identity();
        
		position = Vector3<Float_32>(0,0,0);
        scale = Vector3<Float_32>(1,1,1);
        rotation = Vector3<Float_32>(0,0,0);

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

		rotation_transformation = Matrix4x4<Float_32>::identity();
		rotation_transformation.data[(0 * 4) + 0] = cosf(b) * cosf(c);
		rotation_transformation.data[(0 * 4) + 1] = cosf(b) * sinf(c);
		rotation_transformation.data[(0 * 4) + 2] = -sinf(b);
		rotation_transformation.data[(0 * 4) + 3] = 0;

		rotation_transformation.data[(1 * 4) + 0] = (sinf(a) * sinf(b) * cosf(c)) - (cosf(a) * sinf(c));
		rotation_transformation.data[(1 * 4) + 1] = (sinf(a) * sinf(b) * sinf(c)) + (cosf(a) * cosf(c));
		rotation_transformation.data[(1 * 4) + 2] = sinf(a)*cosf(b);
		rotation_transformation.data[(1 * 4) + 3] = 0;

		rotation_transformation.data[(2 * 4) + 0] = (cosf(a) * sinf(b) * cosf(c)) + (sinf(a) * sinf(c));
		rotation_transformation.data[(2 * 4) + 1] = (cosf(a) * sinf(b) * sinf(c)) - (sinf(a) * cosf(c));
		rotation_transformation.data[(2 * 4) + 2] = cosf(a) * cosf(b);
		rotation_transformation.data[(2 * 4) + 3] = 0;

		rotation_transformation.data[(3 * 4) + 0] = 0;
		rotation_transformation.data[(3 * 4) + 1] = 0;
		rotation_transformation.data[(3 * 4) + 2] = 0;
		rotation_transformation.data[(3 * 4) + 3] = 1;

		Matrix4x4<Float_32> scale_m = Matrix4x4<Float_32>::identity();
		scale_m.set(0, 0, scale.x);
		scale_m.set(1, 1, scale.y);
		scale_m.set(2, 2, scale.z);

		Matrix4x4<Float_32> translation = Matrix4x4<Float_32>::identity();
		translation.set(0, 3, position.x);
		translation.set(1, 3, position.y);
		translation.set(2, 3, position.z);

        transformation = transforms[parent_transform].transformation * translation * rotation_transformation * scale_m;
    }

	void Transform3D::update()
	{
		update_transformation();
	}

    const Vector3<Float_32> const Transform3D::get_forward()
    {
        return rotation_transformation * Vector3<Float_32>(0, 0, 1.f);
    }

    const Vector3<Float_32> Transform3D::get_right()
    {
        return rotation_transformation * Vector3<Float_32>(1.f, 0, 0.f);
    }

    const Vector3<Float_32> Transform3D::get_up()
    {
        return rotation_transformation * Vector3<Float_32>(0, 1.f, 0.f);
    }

	void Transform3D::translate(const Vector3<Float_32>& translation)
	{
		position += translation;
	}

	void Transform3D::rotate(Float_32 _x, Float_32 _y, Float_32 _z)
	{
		rotation.x += _x;
		rotation.y += _y;
		rotation.z += _z;
	}

	
    std::string Transform3D::to_string()
    {
    	std::string text = std::to_string(COMP_TRANSFORM_3D)+",";

		Vector3<Float_32> _position = get_global_position();
		Vector3<Float_32> _rotation = get_global_rotation();

		text += std::to_string(_position.x) + "," + std::to_string(_position.y) + "," + std::to_string(_position.z) + ",";
		text += std::to_string(scale.x) + "," + std::to_string(scale.y) + "," + std::to_string(scale.z) + ",";
		text += std::to_string(_rotation.x) + "," + std::to_string(_rotation.y) + "," + std::to_string(_rotation.z) + ",";
		text += std::to_string(parent_transform);

		return text;
    }


	Vector3<Float_32> Transform3D::get_global_position()
	{
		//Vector3<Float_32> parent_pos;
		Matrix4x4<Float_32> parent_t = Matrix4x4<Float_32>::identity();
		if (parent_transform)
			parent_t = transforms[parent_transform].transformation;
		auto pos = parent_t * position;
		return pos;
	}

	Vector3<Float_32> Transform3D::get_global_scale()
	{
		//Vector3<Float_32> parent_pos;
		Matrix4x4<Float_32> parent_t = Matrix4x4<Float_32>::identity();
		if (parent_transform)
			parent_t = transforms[parent_transform].transformation;
		auto _scale = parent_t * scale;
		return _scale;
	}

	Vector3<Float_32> Transform3D::get_global_rotation()
	{
		Vector3<Float_32> parent_rotation;
		if (parent_transform)
			parent_rotation = transforms[parent_transform].get_global_rotation();

		return parent_rotation + rotation;
	}

	Vector3<Float_32> Transform3D::get_global_to_local_position(Vector3<Float_32> global_pos)
	{
		Matrix4x4<Float_32> parent_t = Matrix4x4<Float_32>::identity();
		Vector3<Float_32> parent_gp;
		if (parent_transform)
		{
			parent_gp = transforms[parent_transform].get_global_position();
			parent_t = transforms[parent_transform].transformation;
		}

		Vector3<Float_32> local_pos = parent_t.transpose() * (global_pos - parent_gp);
		return local_pos;
	}

	Vector3<Float_32> Transform3D::get_global_to_local_rotation(Vector3<Float_32> global_rot)
	{
		Vector3<Float_32> parent_rot;
		if (parent_transform)
			parent_rot = transforms[parent_transform].get_global_rotation();

		Vector3<Float_32> local_rot = global_rot - parent_rot;
		return local_rot;
	}


	
}
