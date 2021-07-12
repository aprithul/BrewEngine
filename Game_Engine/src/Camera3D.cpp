#include "Camera3D.hpp"
#include "EntityManagementSystemModule.hpp"
#include "RendererOpenGL2D.hpp"
#include "Math.hpp"
namespace PrEngine
{

	Camera::Camera():Component(COMP_CAMERA)
	{
		this->h_mod = 1;
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
		this->width = right - left;
		this->height = top - bottom;
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
        //view_matrix = Mat4x4::Identity();
        //view_matrix(0,3) = -transforms[id_transform].position.x;
        //view_matrix(1,3) = -transforms[id_transform].position.y;
        //view_matrix(2,3) = -transforms[id_transform].position.z;
		//Mat4x4 reverse_rot = Mat4x4::Identity();// transforms[id_transform].transformation.GetTransformInverse();
		view_matrix = transform_system.get_component(id_transform).transformation.GetTransformInverse();// reverse_rot * view_matrix;
        
        //if(projection_type==PERSPECTIVE)
       //     projection_matrix = Mat4x4::perspective(near_, far_,width, height, fov);
        //else
            projection_matrix = Mat4x4::Orthogrpahic(h_mod*left*zoom, h_mod*right*zoom, bottom*zoom, top*zoom, near_, far_);
            //projection_matrix = Mat4x4::ortho(0, width,0, height, near_, far_);
    }

    void Camera::end()
    {

    }

    std::string Camera::to_string()
    {
		if (projection_type == ORTHOGRAPHIC)
			return std::to_string(COMP_CAMERA) + "," + std::to_string(projection_type) + "," + std::to_string(left) + "," + std::to_string(right) + "," + std::to_string(bottom) + "," + std::to_string(top) + "," + std::to_string(near_) + "," + std::to_string(far_) + ","+std::to_string(zoom);
		else
			return std::to_string(COMP_CAMERA) + "," + std::to_string(projection_type) + "," + std::to_string(width) + "," + std::to_string(height) + "," + std::to_string(near_) + "," + std::to_string(far_) + "," + std::to_string(fov);
	}


	Vec3f Camera::get_mouse_to_screen_pos(Vec2f mouse_pos)
	{
		return Vec2f{ (Float_32)mouse_pos.x, (Float_32)(renderer->height - mouse_pos.y) };
	}
	Vec3f Camera::get_screen_to_world_pos(Vec2f screen_pos)
	{
		static int _wait = 60;
		_wait--;
		//screen_pos.x = 155;
		//screen_pos.y = 202;
		Int_32 _x = clamp<Float_32>(screen_pos.x, renderer->viewport_pos.x, renderer->viewport_pos.x+renderer->viewport_size.x);
		Int_32 _y = clamp<Float_32>(screen_pos.y, renderer->viewport_pos.y, renderer->viewport_pos.y+renderer->viewport_size.y);

		Int_32 _x_in_v_port = _x - renderer->viewport_pos.x;
		Int_32 _y_in_v_port = _y - renderer->viewport_pos.y;

		Float_32 _x_ndc = ((_x_in_v_port / (Float_32)renderer->viewport_size.x) * 2) - 1.f;
		Float_32 _y_ndc = ((_y_in_v_port / (Float_32)renderer->viewport_size.y) * 2) - 1.f;
		Point3d ndc_co = { _x_ndc, _y_ndc, 0};

		Mat4x4 vp = projection_matrix* view_matrix;
		Point3d world_pos = vp.GetInverse() * ndc_co;

		return Vec3f{ world_pos.x, world_pos.y, world_pos.z };

		/*

		Transform3D& camera_t = transforms[id_transform];
		Float_32 x_pos = h_mod*zoom*(right - left) / renderer->viewport_size.x;// +camera_pos.x;
		Float_32 y_pos = zoom*(top - bottom) / renderer->viewport_size.y;// +camera_pos.y;

		//in viewport co ordinate
		Vec3f pos_in_viewport = Vec2f{ clamp<Float_32>(screen_pos.x - renderer->viewport_pos.x, 0, renderer->viewport_size.x) ,
			clamp<Float_32>((renderer->height - screen_pos.y) - renderer->viewport_pos.y, 0, renderer->viewport_size.y) };

		Float_32 v_x = pos_in_viewport.x - renderer->viewport_size.x / 2;
		Float_32 v_y = pos_in_viewport.y - renderer->viewport_size.y / 2;

		//Float_32 centered = (screen_pos.x - renderer->viewport_size.x / 2);
		x_pos = x_pos * v_x;
		y_pos = y_pos * v_y;// (screen_pos.y - renderer->viewport_size.y / 2);
		//auto mat = camera_t.transformation.transpose();
		Vec4f world_pos =  camera_t.transformation* Vec4f{x_pos, y_pos, 0, 1};
		return (Vec3f)world_pos;
		*/
	}

}
