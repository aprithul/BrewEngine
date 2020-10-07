#include "Camera3D.hpp"
#include "EntityManagementSystemModule.hpp"

namespace PrEngine
{

	Camera::Camera():Component(COMP_CAMERA)
	{

	}

    void Camera::set_perspective(Float_32 width, Float_32 height, Float_32 near_, Float_32 far_, Float_32 fov)
    {  
        this->width = width;
        this->height = height;
        this->near_ = near_;
        this->far_ = far_;
        this->fov = fov;
        this->projection_type = PERSPECTIVE;

    }

    void Camera::set_orthographic(Float_32 left, Float_32 right, Float_32 bottom, Float_32 top, Float_32 near_, Float_32 far_)
    {
        //fov = 45.f;
        //near_ = 0.1f;
        //far_ = -1.f;
        this->left = left;
        this->right = right;
        this->bottom = bottom;
        this->top = top;
        this->near_ = near_;
        this->far_ = far_;
        this->zoom = 1;
        this->projection_type = ORTHOGRAPHIC;

    }

    Camera::~Camera()
    {

    }
    void Camera::awake()
    {

    }

    void Camera::start()
    {
		
    }

    void Camera::update()
    {

        // set view matrix based on camera

        view_matrix = Matrix4x4<Float_32>::identity();
        view_matrix.set(0,3, -transforms[id_transform].position.x);
        view_matrix.set(1,3, -transforms[id_transform].position.y);
        view_matrix.set(2,3, -transforms[id_transform].position.z);
        Matrix4x4<Float_32> reverse_rot = transforms[id_transform].rotation_transformation.transpose();
        view_matrix = reverse_rot * view_matrix;
        
        if(projection_type==PERSPECTIVE)
            projection_matrix = Matrix4x4<Float_32>::perspective(near_, far_,width, height, fov);
        else
            projection_matrix = Matrix4x4<Float_32>::ortho(left*zoom, right*zoom, bottom*zoom, top*zoom, near_, far_);
            //projection_matrix = Matrix4x4<Float_32>::ortho(0, width,0, height, near_, far_);
    }

    void Camera::end()
    {

    }

    std::string Camera::to_string()
    {
		if (projection_type == ORTHOGRAPHIC)
			return std::to_string(COMP_CAMERA) + "," + std::to_string(projection_type) + "," + std::to_string(left) + "," + std::to_string(right) + "," + std::to_string(bottom) + "," + std::to_string(top) + "," + std::to_string(near_) + "," + std::to_string(far_);
		else
			return std::to_string(COMP_CAMERA) + "," + std::to_string(projection_type) + "," + std::to_string(width) + "," + std::to_string(height) + "," + std::to_string(near_) + "," + std::to_string(far_) + "," + std::to_string(fov);
	}
}
