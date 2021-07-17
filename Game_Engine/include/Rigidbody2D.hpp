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
			Float_32 drag;
			Float_32 angular_drag;
			Uint_32 transform_id;
			Uint_32 collider_id;
			Bool_8 is_kinematic;

			Rigidbody2D();
			~Rigidbody2D();
			void initialize() override;
			void start() override;
			void update() override;
			void end() override;
			void add_force(Vec2f _force);
			void add_force_at_point(Vec2f _force, Vec2f _point);
			std::string to_string() override;

	private:
		Vec2f accumulated_force;
		Float_32 accumulated_torque;
		Float_32 inertia_inverse;

	};
}

#endif