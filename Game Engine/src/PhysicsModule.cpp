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
		for (Uint_32 _i = 0; _i<entity_management_system->next_collider_pos; _i++)
		{
			if (collider_entity_id[_i])
			{
				Uint_32 graphic_id = colliders[_i].graphic_id;
				if (graphic_id)
				{
					Vector2<Float_32>* col_points = colliders[_i].collision_shape.points;
					Vector3<Float_32>* g_points = Graphic::vertex_data[graphic_id];
					col_points[0] = g_points[0];
					col_points[1] = g_points[1];
					col_points[2] = g_points[2];
					col_points[3] = g_points[3];
					colliders[_i].collision_shape.type = SHAPE_RECT;
					colliders[_i].collision_shape.point_count = 4;
				}
				else
				{
					Vector2<Float_32>* col_points = colliders[_i].collision_shape.points;
					col_points[0] = Vector2<Float_32>{ -1, 1 };
					col_points[1] = Vector2<Float_32>{  1, 1 };
					col_points[2] = Vector2<Float_32>{  1,-1 };
					col_points[3] = Vector2<Float_32>{ -1,-1 };
					colliders[_i].collision_shape.type = SHAPE_RECT;
					colliders[_i].collision_shape.point_count = 4;

				}
			}
		}


		//// test
		//Vector2<Float_32> col_points[4];
		//col_points[0] = Vector2<Float_32>{ -1, 1 };
		//col_points[1] = Vector2<Float_32>{ 1, 1 };
		//col_points[2] = Vector2<Float_32>{ 1,-1 };
		//col_points[3] = Vector2<Float_32>{ -1,-1 };
		//
		//Bool_8 res = point_in_shape(col_points, 4, Vector2<Float_32>(0,0));

		//return;


	}




	Uint_32 PhysicsModule::point_in_any_shape(Vector2<Float_32> p)
	{
		for (Uint_32 _i = 0; _i < entity_management_system->next_collider_pos; _i++)
		{
			if (collider_entity_id[_i])
			{
				Collider& col = colliders[_i];
				switch (col.collision_shape.type)
				{
				case SHAPE_CIRCLE:
					break;
				case SHAPE_RECT:
				{
					Uint_32 camera_id = entity_management_system->get_active_camera();

					//LOG(LOGTYPE_WARNING, p.to_string());
					Vector2<Float_32> transformed_points[4] = {};
					Transform3D& transform = transforms[col.transform_id];
					for (int _i = 0; _i < 4; _i++)
					{
						transformed_points[_i] = transform.transformation*col.collision_shape.points[_i];
						//transformed_points[_i] = transformed_points[_i] * cameras[camera_id].zoom;
					}
					//for (int _i = 0; _i < 4; _i++)
					//{
					//	renderer->draw_line(transformed_points[_i], transformed_points[(_i + 1) % 4], Vector4<Float_32>{1.0f, 0, 0, 1.0f});
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

	bool PhysicsModule::col_point_box(Vector2<Float_32> p, Vector2<Float_32>* box)
	{
		return false;
	}

	void PhysicsModule::update()
	{
		for (int _i = 0; _i < contacts.size(); _i++)
		{
			Uint_32 col_a = contacts[_i].collider_a;
			Uint_32 tr_a = colliders[col_a].transform_id;
			Uint_32 col_b = contacts[_i].collider_b;
			Uint_32 tr_b = colliders[col_b].transform_id;

			assert(tr_a && tr_b);

			Vector4<Float_32> red_color{ 1.0, 0, 0, 1.0 };
			Rect<Float_32> a = points_to_rect(colliders[col_a].collision_shape.points);//, transforms[tr_a].transformation);
			Rect<Float_32> b = points_to_rect(colliders[col_b].collision_shape.points);//, transforms[tr_b].transformation);
			renderer->draw_rect_with_transform(a, red_color, transforms[tr_a].transformation);
			renderer->draw_rect_with_transform(b, red_color, transforms[tr_b].transformation);
		}

		// all consumed
		contacts.clear();
	}


	void PhysicsModule::end()
	{

	}

}