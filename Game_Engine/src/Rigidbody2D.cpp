#include "Rigidbody2D.hpp"
#include "Transform3D.hpp"
#include "EntityManagementSystemModule.hpp"
#include "TimeModule.hpp"

namespace PrEngine
{
	Rigidbody2D::Rigidbody2D():angular_acceleration(0),angular_velocity(0),Component(COMP_RIGIDBODY_2D)
	{

	}

	Rigidbody2D::~Rigidbody2D()
	{

	}

	void Rigidbody2D::awake()
	{

	}

	void Rigidbody2D::start()
	{

	}

	void Rigidbody2D::update()
	{
		if (transform_id)
		{
			Transform3D& transform = transform_system.get_component(transform_id);
			velocity = velocity + (acceleration * Time::Frame_time);
			Vec3f global_pos = transform.get_global_position();
			Vec2f new_pos =  global_pos + (velocity*Time::Frame_time);
			transform.set_global_position(new_pos.x, new_pos.y, global_pos.z);
		}
	}

	void Rigidbody2D::end()
	{

	}

	std::string Rigidbody2D::to_string()
	{
		return (std::to_string(COMP_RIGIDBODY_2D) + "," + std::to_string(velocity.x) + "," + std::to_string(velocity.y) + "," +
			std::to_string(acceleration.x) + "," + std::to_string(acceleration.y) + "," +
			std::to_string(angular_velocity) + "," + std::to_string(angular_acceleration));
	}
	
}