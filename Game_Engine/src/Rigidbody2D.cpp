#include "Rigidbody2D.hpp"
#include "Transform3D.hpp"
#include "EntityManagementSystemModule.hpp"
#include "TimeModule.hpp"
#include "PhysicsModule.hpp"
#include "RendererOpenGL2D.hpp"

namespace PrEngine
{
	Rigidbody2D::Rigidbody2D():Component(COMP_RIGIDBODY_2D)
	{
		initialize();
	}

	Rigidbody2D::~Rigidbody2D()
	{

	}

	void Rigidbody2D::initialize()
	{
		velocity = {};
		acceleration = {};
		angular_velocity = 0;
		angular_acceleration = 0;
		mass_inverse = 1.0f;
		drag = 0.1f;
		angular_drag = 0.1f;
		is_kinematic = false;
		transform_id = 0;
		collider_id = 0;

		accumulated_force = {};
		accumulated_torque = 0;
		inertia_inverse = 0.f;
	}

	void Rigidbody2D::start()
	{
		
	}

	void Rigidbody2D::update()
	{

		if (!inertia_inverse)
		{
			assert(transform_id);
			Uint_32 entity = transform_system.get_entity(transform_id);
			collider_id = PhysicsModule::collider_system.get_component_id(entity);

			if (collider_id)
			{
				Collider& collider = PhysicsModule::collider_system.get_component(collider_id);
				if (collider.collision_shape.type == SHAPE_RECT)
				{
					Float_32 width = collider.collision_shape.points[0].x - collider.collision_shape.points[1].x;
					Float_32 height = collider.collision_shape.points[1].y - collider.collision_shape.points[2].y;
					Float_32 inertia = (1.f / mass_inverse)* (width*width + height * height) * (1 / 12.f);
					inertia_inverse = 1.f / inertia;
				}
			}
		}

		if (transform_id && !is_kinematic)
		{
			Transform3D& transform = transform_system.get_component(transform_id);
			acceleration = (accumulated_force * mass_inverse);
			velocity = velocity + (acceleration * Time::Frame_time);
			velocity = velocity * (1.0f - clamp<Float_32>(drag * Time::Frame_time, 0.f, 1.f));
			Vec3f global_pos = transform.get_global_position();
			Vec2f new_pos =  global_pos + (velocity*Time::Frame_time);
			transform.set_global_position(new_pos.x, new_pos.y, global_pos.z);

			// angular rotation
			angular_acceleration = inertia_inverse * accumulated_torque;
			angular_velocity = angular_velocity + (angular_acceleration * Time::Frame_time);
			angular_velocity = angular_velocity * (1.0f - clamp<Float_32>(angular_drag * Time::Frame_time, 0.f, 1.f));
			transform.Rotate(0, 0, (Time::Frame_time*angular_velocity*(1/ 0.0174533f)));
		}

		accumulated_force = { 0,0 };
		accumulated_torque = 0;
	}

	void Rigidbody2D::add_force(Vec2f _force)
	{
		accumulated_force += _force;
	}

	void Rigidbody2D::add_force_at_point(Vec2f _force, Vec2f _point)
	{
		Transform3D& transform = transform_system.get_component(transform_id);
		Vec2f point_ws = transform.transformation * _point;
		Vec2f cofm = transform.get_global_position();
		renderer->draw_line(cofm, point_ws, { 0,0,1,1 });
		accumulated_torque += Cross(_force, point_ws-cofm);
	}

	void Rigidbody2D::end()
	{

	}

	std::string Rigidbody2D::to_string()
	{
		return (std::to_string(COMP_RIGIDBODY_2D) + "," + std::to_string(velocity.x) + "," + std::to_string(velocity.y) + "," +
			std::to_string(acceleration.x) + "," + std::to_string(acceleration.y) + "," +
			std::to_string(angular_velocity) + "," + std::to_string(angular_acceleration) + 
			"," + std::to_string(mass_inverse) + "," + std::to_string(is_kinematic)+","+
			std::to_string(drag) + "," + std::to_string(angular_drag));
	}
	
}