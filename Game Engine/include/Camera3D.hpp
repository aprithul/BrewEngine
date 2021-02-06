#ifndef CAMERA_3D_HPP
#define CAMERA_3D_HPP

//#include "Entity.hpp"
#include "InputModule.hpp"
#include "TimeModule.hpp"
#include "Vector3.hpp"
#include "Transform3D.hpp"

namespace PrEngine
{
    enum ProjectionType
    {
        PERSPECTIVE = 0,
        ORTHOGRAPHIC = 1
    };

    struct Camera: public Component
    {
		Camera();
		void set_perspective(Float_32 width, Float_32 height, Float_32 near, Float_32 far, Float_32 fov);
		void set_orthographic(Float_32 left, Float_32 right, Float_32 bottom, Float_32 top, Float_32 near_, Float_32 far_);
		~Camera();
		Uint_32 id_transform;

		Vector3<Float_32> get_screen_to_world_pos(Vector2<Int_32> screen_pos);
		void zoom_in(Float_32 zoom_speed);
		void zoom_out(Float_32 zoom_speed);
		void awake() override;
		void start() override;
		void update() override;
		void end() override;     
		std::string to_string() override;

		Float_32 fov;
		Float_32 near_;
		Float_32 far_; 
		Float_32 width;
		Float_32 height;

		Float_32 left;
		Float_32 right;
		Float_32 bottom;
		Float_32 top;
		Float_32 zoom;

		Matrix4x4<Float_32> view_matrix;
		Matrix4x4<Float_32> projection_matrix;
		ProjectionType projection_type;



    };
}
#endif
