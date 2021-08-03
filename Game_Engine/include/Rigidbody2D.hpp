#ifndef RIGIDBODY_2D_HPP
#define RIGIDBODY_2D_HPP
#include "Vec.hpp"
#include "Component.hpp"
namespace PrEngine
{
	struct Rigidbody2D : Component
	{
		Vec2f position;
		Float_32 rotation;

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
		Float_32 inertia_inverse;
		Float_32 static_friction;
		Float_32 dynamic_friction;
		Float_32 coefficient_of_restitution;

		Rigidbody2D();
		~Rigidbody2D();
		Rigidbody2D(const Rigidbody2D& other);
		void initialize() override;
		void start() override;
		void update(Float_32 dt) override;
		void end() override;
		void calculate_inertia();
		void add_force(Vec2f _force);
		void add_force_at_point(Vec2f _force, Vec2f _point);
		void add_impulse(Vec2f _impulse);
		void add_impulse_at_point(Vec2f _impulse, Vec2f _point);
		std::string to_string() override;

	private:
		Vec2f accumulated_force;
		Float_32 accumulated_torque;
		bool calculated_inertia;

	};
}

#endif