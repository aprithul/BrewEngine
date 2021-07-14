#ifndef RIGIDBODY_2D_HPP
#define RIGIDBODY_2D_HPP
#include "Vec.hpp"
#include "Component.hpp"
namespace PrEngine
{
	struct Rigidbody2D : Component
	{
			Vec2f velocity;
			Vec2f acceleration;
			Float_32 angular_velocity;
			Float_32 angular_acceleration;
			Float_32 mass_inverse;
			Uint_32 transform_id;
			Bool_8 is_kinematic;

			Rigidbody2D();
			~Rigidbody2D();
			void awake() override;
			void start() override;
			void update() override;
			void end() override;

			std::string to_string() override;

	private:
		Vec3f accumulated_force;

	};
}

#endif