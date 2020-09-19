#include "Camera3D.hpp"

namespace PrEngine
{
    Camera::Camera(float width, float height, float near_, float far_, float fov, Transform3D& _transform):transform(_transform),Component(COMP_CAMERA)
    {
        //fov = 45.f;
        //near_ = 0.1f;
        //far_ = -1.f;    
        this->width = width;
        this->height = height;
        this->near_ = near_;
        this->far_ = far_;
        this->fov = fov;
        this->projection_type = PERSPECTIVE;

    }

    Camera::Camera(float left, float right, float bottom, float top, float near_, float far_, Transform3D& _transform):transform(_transform),Component(COMP_CAMERA)
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
        LOG(LOGTYPE_GENERAL, "Camera started");

    }

    void Camera::update()
    {

        // set view matrix based on camera

        view_matrix = Matrix4x4<float>::identity();
        view_matrix.set(0,3, -transform.get_position().x);
        view_matrix.set(1,3, -transform.get_position().y);
        view_matrix.set(2,3, -transform.get_position().z);
        Matrix4x4<float> reverse_rot = transform.get_rotation_transformation().transpose();
        view_matrix = reverse_rot * view_matrix;
        
        if(projection_type==PERSPECTIVE)
            projection_matrix = Matrix4x4<float>::perspective(near_, far_,width, height, fov);
        else
            projection_matrix = Matrix4x4<float>::ortho(left*zoom, right*zoom, bottom*zoom, top*zoom, near_, far_);
            //projection_matrix = Matrix4x4<float>::ortho(0, width,0, height, near_, far_);
        

/*
        Vector3<float> rot = transform.get_rotation();
        if(input_manager->keyboard.get_key(SDLK_a))
            rot.y = rot.y-(Time::Frame_time*20.f);
        if(input_manager->keyboard.get_key(SDLK_d))
            rot.y = rot.y+(Time::Frame_time*20.f);
        transform.set_rotation(rot);
        if(input_manager->keyboard.get_key(SDLK_z))
            rot.z = rot.z-(Time::Frame_time*20.f);
        if(input_manager->keyboard.get_key(SDLK_c))
            rot.z = rot.z+(Time::Frame_time*20.f);

        if(input_manager->keyboard.get_key(SDLK_q))
            rot.x = rot.x-(Time::Frame_time*20.f);
        if(input_manager->keyboard.get_key(SDLK_e))
            rot.x = rot.x+(Time::Frame_time*20.f);
        transform.set_rotation(rot);*/

        //std::cout<<"Mouse delta: "<<input_manager->mouse.delta.length()<<std::endl;
        /*
        float rotation_factor = 15.f;
        Vector3<float> rot = transform.get_rotation();
        //if(input_manager->keyboard.get_key(SDLK_a))
        rot.y = rot.y+(Time::Frame_time*rotation_factor*input_manager->mouse.delta.x);
        rot.x = rot.x+(Time::Frame_time*rotation_factor*input_manager->mouse.delta.y);
        //if(input_manager->keyboard.get_key(SDLK_d))
        //    rot.y = rot.y-(Time::Frame_time*100.f);
        transform.set_rotation(rot);*/

    }

    void Camera::end()
    {

    }

    std::string Camera::to_string()
    {
    	return std::to_string(COMP_CAMERA);
    }
}
