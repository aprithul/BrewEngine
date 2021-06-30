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
		//Mat4x4 rotation_transformation;
		
		Uint_32 parent_transform;
		
		Transform3D();
		~Transform3D() override;
		void update() override;
		std::string to_string() override;

		void Translate(const Vec3f& translation);
		void Rotate(Float_32 _x, Float_32 _y, Float_32 _z);
		void Scale(Float_32 _x, Float_32 _y, Float_32 _z);

		void set_position(Float_32 _x, Float_32 _y, Float_32 _z);
		void set_position(Point3d position);
		void set_rotation(Float_32 _x, Float_32 _y, Float_32 _z);
		void set_rotation(Vec3f _rotation);
		void set_scale(Float_32 _x, Float_32 _y, Float_32 _z);
		void set_scale(Vec3f _scale);

		Point3d get_position();
		Vec3f get_rotation();
		Vec3f get_scale();


		void update_transformation();


		const Vec3f get_forward();
		const Vec3f get_right();
		const Vec3f get_up();

		Point3d get_global_position();
		Vec3f get_global_rotation();
		Vec3f get_global_scale();
		Point3d get_global_to_local_position(Point3d global_pos);
		Vec3f get_global_to_local_rotation(Vec3f global_rot);

	private :
		Point3d position;
		Vec3f rotation;
		Vec3f scale;
		Bool_8 is_dirty;
    };

	extern Transform3D transforms[];

}
#endif
