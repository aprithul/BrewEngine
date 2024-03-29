#ifndef CAMERA_3D_HPP
#define CAMERA_3D_HPP

//#include "Entity.hpp"
#include "InputModule.hpp"
#include "TimeModule.hpp"
#include "Vec.hpp"
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

		Vec3f get_mouse_to_screen_pos(Vec2f mouse_pos);
		Vec3f get_screen_to_world_pos(Vec2f screen_pos);
		void zoom_in(Float_32 zoom_speed);
		void zoom_out(Float_32 zoom_speed);
		void initialize() override;
		void start() override;
		void update(Float_32 dt) override;
		void end() override;     
		std::string to_string() override;

		Mat4x4 view_matrix;
		Mat4x4 projection_matrix;
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


		ProjectionType projection_type;



    };
}
#endif
