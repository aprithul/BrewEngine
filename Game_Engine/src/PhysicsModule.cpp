#include "PhysicsModule.hpp"
#include "EntityManagementSystemModule.hpp"
#include "RendererOpenGL2D.hpp"
namespace PrEngine {

	PhysicsModule* physics_module = nullptr;

	std::vector<Contact> PhysicsModule::contacts;

	PhysicsModule::PhysicsModule(std::string name, Int_32 priority) :Module(name, priority)
	{
		if(!physics_module)
			physics_module = this;
	}

	PhysicsModule::~PhysicsModule()
	{

	}

	void PhysicsModule::start()
	{
		for (Uint_32 _i = 0; _i<collider_system.new_pos; _i++)
		{
			if (collider_system.get_entity(_i))
			{
				Collider& collider = collider_system.get_component(_i);
				Uint_32 graphic_id = collider.graphic_id;// colliders[_i].graphic_id;
				if (graphic_id)
				{
					Vec2f* col_points = collider.collision_shape.points;
					Vec3f* g_points = Graphic::vertex_data[graphic_id];
					col_points[0] = g_points[0];
					col_points[1] = g_points[1];
					col_points[2] = g_points[2];
					col_points[3] = g_points[3];
					collider.collision_shape.type = SHAPE_RECT;
					collider.collision_shape.point_count = 4;
				}
				else
				{
					Vec2f* col_points = collider.collision_shape.points;
					col_points[0] = Vec2f{ -1, 1 };
					col_points[1] = Vec2f{  1, 1 };
					col_points[2] = Vec2f{  1,-1 };
					col_points[3] = Vec2f{ -1,-1 };
					collider.collision_shape.type = SHAPE_RECT;
					collider.collision_shape.point_count = 4;

				}
			}
		}


		//// test
		//Vec2f col_points[4];
		//col_points[0] = Vec2f{ -1, 1 };
		//col_points[1] = Vec2f{ 1, 1 };
		//col_points[2] = Vec2f{ 1,-1 };
		//col_points[3] = Vec2f{ -1,-1 };
		//
		//Bool_8 res = point_in_shape(col_points, 4, Vec2f(0,0));

		//return;


	}




	Uint_32 PhysicsModule::point_in_any_shape(Vec2f p)
	{
		for (Uint_32 _i = 0; _i < collider_system.new_pos; _i++)
		{
			if (collider_system.get_entity(_i))
			{
				Collider& col = collider_system.get_component(_i);// colliders[_i];
				switch (col.collision_shape.type)
				{
				case SHAPE_CIRCLE:
					break;
				case SHAPE_RECT:
				{
					Uint_32 camera_id = entity_management_system->get_active_camera();

					//LOG(LOGTYPE_WARNING, p.to_string());
					Vec2f transformed_points[4] = {};
					Transform3D& transform = transform_system.get_component(col.transform_id);
					for (int _i = 0; _i < 4; _i++)
					{
						transformed_points[_i] = transform.transformation*col.collision_shape.points[_i];
						//transformed_points[_i] = transformed_points[_i] * cameras[camera_id].zoom;
					}
					//for (int _i = 0; _i < 4; _i++)
					//{
					//	renderer->draw_line(transformed_points[_i], transformed_points[(_i + 1) % 4], Vec4f{1.0f, 0, 0, 1.0f});
					//}
					if (point_in_shape(transformed_points, 4, p))
					{
						return col.transform_id;
					}

				}
					break;
				case SHAPE_POLY:
					break;
				case SHAPE_POINT:
					break;
				default:
					break;
				}
			}
		}
		return 0;
	}

	bool PhysicsModule::col_point_box(Vec2f p, Vec2f* box)
	{
		return false;
	}

	void PhysicsModule::update()
	{
		auto _s = contacts.size();
		for (int _i = 0; _i < contacts.size(); _i++)
		{
			Uint_32 col_a = contacts[_i].collider_a;
			Uint_32 tr_a = collider_system.get_component(col_a).transform_id;
			Uint_32 col_b = contacts[_i].collider_b;
			Uint_32 tr_b = collider_system.get_component(col_b).transform_id;

			assert(tr_a && tr_b);

			Vec4f red_color{ 1.0, 0, 0, 1.0 };
			Vec4f yellow_color{ 0.0, 1, 1, 1.0 };
			Rect<Float_32> a = points_to_rect(collider_system.get_component(col_a).collision_shape.points);//, transforms[tr_a].transformation);
			Rect<Float_32> b = points_to_rect(collider_system.get_component(col_b).collision_shape.points);//, transforms[tr_b].transformation);
			renderer->draw_rect_with_transform(a, red_color, transform_system.get_component(tr_a).transformation);
			renderer->draw_rect_with_transform(b, red_color, transform_system.get_component(tr_b).transformation);
			renderer->draw_line(transform_system.get_component(tr_b).get_local_position(), transform_system.get_component(tr_b).get_local_position() + (Vec3f)contacts[_i].depth, yellow_color);

			Float_32 _len = contacts[_i].depth.GetMagnitude();

			transform_system.get_component(tr_a).Translate(-(Vec3f)(contacts[_i].depth*0.55));
			//transforms[tr_a].update_transformation();
			transform_system.get_component(tr_b).Translate((Vec3f)(contacts[_i].depth*0.55));
			//transforms[tr_b].update_transformation();
		/*	Float_32 _x = abs<Float_32>(transforms[tr_a].get_position().x);
			_x = abs<Float_32>(transforms[tr_b].get_position.x);
			continue;*/
			//LOG(LOGTYPE_GENERAL, std::to_string(contacts[_i].depth.length()));

		}

		// all consumed
		contacts.clear();
	}


	void PhysicsModule::end()
	{

	}

}