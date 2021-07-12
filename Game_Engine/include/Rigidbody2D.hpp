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
			Uint_32 transform_id;

			Rigidbody2D();
			~Rigidbody2D();
			void awake() override;
			void start() override;
			void update() override;
			void end() override;

			std::string to_string() override;

	};
}

#endif