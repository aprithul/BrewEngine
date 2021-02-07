#ifndef TRANSFORM3D_HPP
#define TRANSFORM3D_HPP

#include "Vector3.hpp"
#include "Logger.hpp"
#include "Component.hpp"
#include "Matrix4x4f.hpp"
#include "Serializable.hpp"
#include "Types.hpp"
#include <cmath>
#include <string>

#define PI 3.14159265

namespace PrEngine{
    
    struct Transform3D : public Component
    {
		Matrix4x4<Float_32> transformation;
		Matrix4x4<Float_32> rotation_transformation;
		Vector3<Float_32> position;
		Vector3<Float_32> rotation;
		Vector3<Float_32> scale;
		Uint_32 parent_transform;
		
		Transform3D();
		~Transform3D() override;
		void update() override;
		std::string to_string() override;

		void translate(const Vector3<Float_32>& translation);
		void rotate(Float_32 _x, Float_32 _y, Float_32 _z);
		void update_transformation();

		const Vector3<Float_32> get_forward();
		const Vector3<Float_32> get_right();
		const Vector3<Float_32> get_up();
	
    };

	extern Transform3D transforms[];

}
#endif
