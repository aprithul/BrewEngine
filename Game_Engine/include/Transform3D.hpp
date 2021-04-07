#ifndef TRANSFORM3D_HPP
#define TRANSFORM3D_HPP

#include "Vec.hpp"
#include "Logger.hpp"
#include "Component.hpp"
//#include "Matrix4x4f.hpp"
#include "Serializable.hpp"
#include "Types.hpp"
#include <cmath>
#include "Math.hpp"
#include <string>


namespace PrEngine{
    
    struct Transform3D : public Component
    {
		Mat4x4 transformation;
		Mat4x4 rotation_transformation;
		Vec3f position;
		Vec3f rotation;
		Vec3f scale;
		Uint_32 parent_transform;
		
		Transform3D();
		~Transform3D() override;
		void update() override;
		std::string to_string() override;

		void translate(const Vec3f& translation);
		void rotate(Float_32 _x, Float_32 _y, Float_32 _z);
		void update_transformation();

		const Vec3f get_forward();
		const Vec3f get_right();
		const Vec3f get_up();

		Vec3f get_global_position();
		Vec3f get_global_rotation();
		Vec3f get_global_scale();
		Vec3f get_global_to_local_position(Vec3f global_pos);
		Vec3f get_global_to_local_rotation(Vec3f global_rot);
    };

	extern Transform3D transforms[];

}
#endif
