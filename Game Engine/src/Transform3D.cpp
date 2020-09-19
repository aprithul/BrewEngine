#include "Transform3D.hpp"
namespace  PrEngine
{
    Transform3D::Transform3D():Component(COMP_TRANSFORM_3D)
    {
        translation = Matrix4x4<float>::identity();
        scale_m = Matrix4x4<float>::identity();
        rotation_x = Matrix4x4<float>::identity();
        rotation_y = Matrix4x4<float>::identity();
        rotation_z = Matrix4x4<float>::identity();
        transformation = Matrix4x4<float>::identity();
        rotation_transformation = Matrix4x4<float>::identity();
        position = Vector3<float>(0,0,0);
        scale = Vector3<float>(1,1,1);
        rotation = Vector3<float>(0,0,0);
        forward = Vector3<float>(0,0,1.f);
        right   = Vector3<float>(1.f,0,0.f);
        up      = Vector3<float>(0,1.f,0.f);
        dirty = true;

        LOG(LOGTYPE_WARNING, "CALLED");
    }

    Transform3D::~Transform3D()
    {

    }

    void Transform3D::set_position(const Vector3<float>& position)
    {
        this->position = position;
        translation.set(0,3, this->position.x);
        translation.set(1,3, this->position.y);
        translation.set(2,3, this->position.z);
        update_transformation();
    }
    
    void Transform3D::set_position(float x, float y, float z)
    {
        this->position.x = x;
        this->position.y = y;
        this->position.z = z;
        translation.set(0,3, this->position.x);
        translation.set(1,3, this->position.y);
        translation.set(2,3, this->position.z);
        update_transformation();
    }

    void Transform3D::set_scale(const Vector3<float>& scale)
    {
        this->scale = scale;
        scale_m.set(0,0, this->scale.x);
        scale_m.set(1,1, this->scale.y);
        scale_m.set(2,2, this->scale.z);
        update_transformation();
    }
    
    void Transform3D::set_scale(float x, float y, float z)
    {
        this->scale.x = x;
        this->scale.y = y;
        this->scale.z = z;
        scale_m.set(0,0, this->scale.x);
        scale_m.set(1,1, this->scale.y);
        scale_m.set(2,2, this->scale.z);
        update_transformation();
    }

    void Transform3D::set_rotation(const Vector3<float>& rotation)
    {
        this->rotation = rotation;

        rotation_x.set(1,1, cosf(rotation.x*PI/180.f));
        rotation_x.set(1,2, -sinf(rotation.x*PI/180.f));
        rotation_x.set(2,1, sinf(rotation.x*PI/180.f));
        rotation_x.set(2,2, cosf(rotation.x*PI/180.f));
        
        rotation_y.set(0,0, cosf(rotation.y*PI/180.f));
        rotation_y.set(0,2, sinf(rotation.y*PI/180.f));
        rotation_y.set(2,0, -sinf(rotation.y*PI/180.f));
        rotation_y.set(2,2, cosf(rotation.y*PI/180.f));

        rotation_z.set(0,0, cosf(rotation.z*PI/180.f));
        rotation_z.set(0,1, -sinf(rotation.z*PI/180.f));
        rotation_z.set(1,0, sinf(rotation.z*PI/180.f));
        rotation_z.set(1,1, cosf(rotation.z*PI/180.f));
        update_transformation();
    }
    
    void Transform3D::set_rotation(float x, float y, float z)
    {
        this->rotation.x = x;
        this->rotation.y = y;
        this->rotation.z = z;

        rotation_x.set(1,1, cosf(rotation.x*PI/180.f));
        rotation_x.set(1,2, -sinf(rotation.x*PI/180.f));
        rotation_x.set(2,1, sinf(rotation.x*PI/180.f));
        rotation_x.set(2,2, cosf(rotation.x*PI/180.f));
        
        rotation_y.set(0,0, cosf(rotation.y*PI/180.f));
        rotation_y.set(0,2, sinf(rotation.y*PI/180.f));
        rotation_y.set(2,0, -sinf(rotation.y*PI/180.f));
        rotation_y.set(2,2, cosf(rotation.y*PI/180.f));

        rotation_z.set(0,0, cosf(rotation.z*PI/180.f));
        rotation_z.set(0,1, -sinf(rotation.z*PI/180.f));
        rotation_z.set(1,0, sinf(rotation.z*PI/180.f));
        rotation_z.set(1,1, cosf(rotation.z*PI/180.f));
        update_transformation();
    }

    void Transform3D::update_transformation()
    {
        rotation_transformation = rotation_z * rotation_y* rotation_x;
        transformation = translation * rotation_transformation * scale_m;
        dirty = true;
    }

    const Matrix4x4<float>& Transform3D::get_transformation()
    {
        return transformation;
    }

    const Matrix4x4<float>& Transform3D::get_translation_transformation()
    {
        return translation;
    }

    const Matrix4x4<float>& Transform3D::get_rotation_transformation()
    {
        return rotation_transformation;
    }

    const Vector3<float>& Transform3D::get_scale()
    {
        return scale;
    }

    const Vector3<float>& Transform3D::get_position()
    {
        return position;
    }

    const Vector3<float>& Transform3D::get_rotation()
    {
        return rotation;
    }

    const Vector3<float> Transform3D::get_forward()
    {
        return rotation_transformation * forward;
    }

    const Vector3<float> Transform3D::get_right()
    {
        return rotation_transformation * right;
    }

    const Vector3<float> Transform3D::get_up()
    {
        return rotation_transformation * up;
    }

    std::string Transform3D::to_string()
    {
    	std::string text = std::to_string(COMP_TRANSFORM_3D)+",";
    	text+=std::to_string(position.x)+","+std::to_string(position.y)+","+std::to_string(position.z)+",";
		text+=std::to_string(scale.x)+","+std::to_string(scale.y)+","+std::to_string(scale.z)+",";
		text+=std::to_string(rotation.x)+","+std::to_string(rotation.y)+","+std::to_string(rotation.z);
		return text;

    }
}
