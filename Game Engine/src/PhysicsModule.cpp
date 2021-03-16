#include "PhysicsModule.hpp"
#include "EntityManagementSystemModule.hpp"
#include "RendererOpenGL2D.hpp"
namespace PrEngine {

	PhysicsModule* physics_module = nullptr;

	//std::vector<CollisionShape2D> PhysicsModule::collision_shapes;

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
				}
				else
				{
					Vector2<Float_32>* col_points = colliders[_i].collision_shape.points;
					col_points[0] = Vector2<Float_32>{ -1, 1 };
					col_points[1] = Vector2<Float_32>{  1, 1 };
					col_points[2] = Vector2<Float_32>{  1,-1 };
					col_points[3] = Vector2<Float_32>{ -1,-1 };
					colliders[_i].collision_shape.type = SHAPE_RECT;
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

	Bool_8 PhysicsModule::point_in_AABB(Vector2<Int_32> p, Rect& rect)
	{
		if (p.x > rect.x && p.x < rect.x + rect.w
			&& p.y > rect.y && p.y < rect.y + rect.h)
			return true;
		else
			return false;
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

		/*for (Uint_32 _i = 0; _i < collision_shapes.size(); _i++)
		{
			for (Uint_32 _j = 1; _j < collision_shapes.size(); _j++)
			{

			}
		}*/
	}


	void PhysicsModule::end()
	{

	}

}