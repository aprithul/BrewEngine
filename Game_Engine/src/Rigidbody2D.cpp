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

		Vec2f position = { 0,0 };
		Float_32 rotation = 0;

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
		inertia_inverse = 0.f;
		coefficient_of_restitution = 0.f;
		static_friction = 0;
		dynamic_friction = 0;

		accumulated_force = {};
		accumulated_torque = 0;
		calculated_inertia = false;
	}

	void Rigidbody2D::start()
	{
		
	}

	void Rigidbody2D::update(Float_32 dt)
	{

		if (!calculated_inertia)
		{
			calculate_inertia();
			calculated_inertia = true;
		}

		if (transform_id && !is_kinematic)
		{
			//Transform3D& transform = transform_system.get_component(transform_id);
			acceleration = (accumulated_force * mass_inverse);
			//acceleration += PhysicsModule::gravity;

			velocity = velocity + (acceleration * dt);
			position = position + (velocity*dt);

			angular_velocity = angular_velocity + (angular_acceleration * dt);
			rotation = rotation + (angular_velocity*dt);

			//velocity = velocity * (1.0f - clamp<Float_32>(drag * dt, 0.f, 1.f));
			//Vec3f global_pos = transform.get_global_position();
			//Vec2f new_pos =  global_pos + (velocity*dt);
			//transform.set_global_position(new_pos.x, new_pos.y, global_pos.z);

			// angular rotation
			//angular_acceleration = inertia_inverse * accumulated_torque;
			//angular_velocity = angular_velocity + (angular_acceleration * dt);
			//angular_velocity = angular_velocity * (1.0f - clamp<Float_32>(angular_drag * dt, 0.f, 1.f));
			//transform.Rotate(0, 0, angular_velocity*dt*57.2958f);
			//LOG(LOGTYPE_GENERAL, std::to_string(velocity.y));
			//transform.update_transformation(false);
		}

		accumulated_force = { 0,0 };
		accumulated_torque = 0;
		acceleration = { 0,0 };
		angular_acceleration = 0;
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

	void Rigidbody2D::add_impulse(Vec2f _impulse)
	{
		if (!is_kinematic)
		{
			velocity += _impulse * mass_inverse;
		}
	}

	void Rigidbody2D::add_impulse_at_point(Vec2f _impulse, Vec2f _point)
	{
		if (!is_kinematic)
		{
			//Transform3D& transform = transform_system.get_component(transform_id);
			//Vec2f point_ws = transform.transformation * _point;
			Vec2f r = _point - position;
			angular_velocity = angular_velocity + (inertia_inverse * Cross(r, _impulse));
		}
		//renderer->draw_line(cofm, point_ws, { 0,0,1,1 });
		//accumulated_torque += Cross(_force, point_ws - cofm);
		//angular_velocity += (_impulse * inertia_inverse);
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
			std::to_string(drag) + "," + std::to_string(angular_drag) +","+ std::to_string(static_friction)
			+ "," + std::to_string(dynamic_friction) + ","+ std::to_string(coefficient_of_restitution));
	}


	void Rigidbody2D::calculate_inertia()
	{
		assert(transform_id);
		Uint_32 entity = transform_system.get_entity(transform_id);
		collider_id = PhysicsModule::collider_system.get_component_id(entity);
		Transform3D& _transform = transform_system.get_component(transform_id);
		_transform.update_transformation(false);
		if (collider_id)
		{
			Collider& collider = PhysicsModule::collider_system.get_component(collider_id);
			if (collider.type == SHAPE_RECT)
			{
				Vec3f p0 = _transform.transformation * collider.points[0];
				Vec3f p1 = _transform.transformation * collider.points[1];
				Vec3f p2 = _transform.transformation * collider.points[2];

				Float_32 width = p0.x - p1.x;
				Float_32 height = p1.y - p2.y;
				Float_32 _mass = 1 / mass_inverse;
				//if (is_kinematic)
				//	_mass = 10000000.f;
				
				Float_32 inertia = _mass * (width*width + height * height) * (1 / 12.f);
				inertia_inverse = 1.f / inertia;
			}
			else if (collider.type == SHAPE_CIRCLE)
			{
				Float_32 inertia = 0.5f * (1 / mass_inverse) * (collider.points[0].x * collider.points[0].x);
				inertia_inverse = 1.f / inertia;
			}
			else if (collider.type == SHAPE_POLY)
			{
				Float_32 inertia = 0.166667f;
				inertia_inverse = 1.f / inertia;
			}
		}
	}
	
}