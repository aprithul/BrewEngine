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
		std::vector<Uint_32>* children;
		Bool_8 is_dirty;

		Transform3D();
		~Transform3D() override;
		void initialize() override;
		void start() override;
		void update() override;
		void end() override;

		std::string to_string() override;

		void Translate(const Vec3f& translation);
		void Rotate(Float_32 _x, Float_32 _y, Float_32 _z);
		void Scale(Float_32 _x, Float_32 _y, Float_32 _z);

		void set_local_position(Float_32 _x, Float_32 _y, Float_32 _z);
		void set_local_position(Point3d position);
		void set_local_rotation(Float_32 _x, Float_32 _y, Float_32 _z);
		void set_local_rotation(Vec3f _rotation);
		void set_local_rotation(Quaternion _rotation);
		void set_local_scale(Float_32 _x, Float_32 _y, Float_32 _z);
		void set_local_scale(Vec3f _scale);

		void set_global_position(Float_32 _x, Float_32 _y, Float_32 _z);
		void set_global_position(Point3d position);
		void set_global_rotation(Float_32 _x, Float_32 _y, Float_32 _z);
		void set_global_rotation(Quaternion _rotation);
		void set_global_scale(Float_32 _x, Float_32 _y, Float_32 _z);
		void set_global_scale(Vec3f _scale);

		Point3d get_local_position();
		const Quaternion& get_local_rotation();
		Vec3f get_local_scale();


		void update_transformation(Bool_8 is_parent_dirty);


		const Vec3f get_forward();
		const Vec3f get_right();
		const Vec3f get_up();

		Point3d get_global_position();
		Quaternion get_global_rotation();
		Vec3f get_global_scale();
		/*Point3d get_global_to_local_position(Point3d global_pos);
		Vec3f get_global_to_local_rotation(Vec3f global_rot);*/

	private :
		Point3d local_position;
		Quaternion local_rotation;
		Vec3f local_scale;
    };

	//extern Transform3D transforms[];

}
#endif
