#include "PhysicsModule.hpp"
#include "EntityManagementSystemModule.hpp"
#include "RendererOpenGL2D.hpp"
namespace PrEngine {

	ComponentSystem<Rigidbody2D> PhysicsModule::rigidbody2d_system(Max_rigidbody2d_count);
	ComponentSystem<Collider> PhysicsModule::collider_system(Max_collider_count);
	Vec2f PhysicsModule::gravity;

	PhysicsModule* physics_module = nullptr;

	std::vector<Contact> PhysicsModule::contacts;

	PhysicsModule::PhysicsModule(std::string name, Int_32 priority) :Module(name, priority)
	{
		if(!physics_module)
			physics_module = this;

		gravity = { 0, -9.8f };
	}

	PhysicsModule::~PhysicsModule()
	{

	}

	void PhysicsModule::start()
	{
		rigidbody2d_system.start();

		//for (Uint_32 _i = 0; _i<collider_system.new_pos; _i++)
		//{
		//	if (collider_system.get_entity(_i))
		//	{
		//		Collider& collider = collider_system.get_component(_i);
		//		Uint_32 graphic_id = collider.graphic_id;// colliders[_i].graphic_id;
		//		if (graphic_id)
		//		{
		//			Vec2f* col_points = collider.collision_shape.points;
		//			Vec3f* g_points = Graphic::vertex_data[graphic_id];
		//			col_points[0] = g_points[0];
		//			col_points[1] = g_points[1];
		//			col_points[2] = g_points[2];
		//			col_points[3] = g_points[3];
		//			collider.collision_shape.type = SHAPE_RECT;
		//			collider.collision_shape.point_count = 4;
		//		}
		//		else
		//		{
		//			Vec2f* col_points = collider.collision_shape.points;
		//			col_points[0] = Vec2f{ -1, 1 };
		//			col_points[1] = Vec2f{  1, 1 };
		//			col_points[2] = Vec2f{  1,-1 };
		//			col_points[3] = Vec2f{ -1,-1 };
		//			collider.collision_shape.type = SHAPE_RECT;
		//			collider.collision_shape.point_count = 4;

		//		}
		//	}
		//}
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

		// update rigidbodies
		rigidbody2d_system.update();

		Uint_32 iterations = 1;
		
		while (iterations-- > 0)
		{		// do collision check
			for (Uint_32 i = 0; i < collider_system.new_id; i++)
			{
				if (collider_system.get_entity(i))
				{
					Uint_32 t_id_i = collider_system.get_component(i).transform_id;
					if (!t_id_i)continue;

					for (Uint_32 j = i + 1; j < collider_system.new_id; j++)
					{
						if (collider_system.get_entity(j))
						{
							Uint_32 t_id_j = collider_system.get_component(j).transform_id;
							if (!t_id_j)continue;
							//Rect<Float_32> r2 = points_to_rect_with_transform(colliders[j].collision_shape.points, transforms[t_id_j].transformation);

							//clock_t begin = clock();
							Bool_8 did_intersect = intersect_GJK(collider_system.get_component(i), collider_system.get_component(j));
							//clock_t end = clock();
							//Double_64 elapsed = (Double_64)(end - begin) / CLOCKS_PER_SEC;

							if (did_intersect)
							{
								//clock_t begin = clock();
								Vec2f col_pen_vec = do_EPA(collider_system.get_component(i), collider_system.get_component(j));
								//clock_t end = clock();
								//Double_64 elapsed = (Double_64)(end - begin) / CLOCKS_PER_SEC;

								physics_module->contacts.push_back(Contact{ col_pen_vec, i, j });
							}
							/*
													if (intersect_AABB_AABB(r1, r2))
													{
														physics_module->contacts.push_back(Contact{ i, j });
													}*/
						}
					}

				}
			}

			for (int _i = 0; _i < contacts.size(); _i++)
			{
				Collider& collider_a = collider_system.get_component(contacts[_i].collider_a);
				Transform3D& transform_a = transform_system.get_component(collider_a.transform_id);
				Collider& collider_b = collider_system.get_component(contacts[_i].collider_b);
				Transform3D& transform_b = transform_system.get_component(collider_b.transform_id);

				//assert(tr_a && tr_b);

				Vec4f red_color{ 1.0, 0, 0, 1.0 };
				Vec4f cyan_color{ 0.0, 1, 1, 1.0 };
				Rect<Float_32> a = points_to_rect(collider_a.collision_shape.points);//, transforms[tr_a].transformation);
				Rect<Float_32> b = points_to_rect(collider_b.collision_shape.points);//, transforms[tr_b].transformation);
				renderer->draw_rect_with_transform(a, red_color, transform_a.transformation);
				renderer->draw_rect_with_transform(b, red_color, transform_b.transformation);
				renderer->draw_line(transform_a.get_global_position(), transform_a.get_global_position() + (Vec3f)contacts[_i].depth, cyan_color);

				Float_32 _len = contacts[_i].depth.GetMagnitude();

				Uint_32 t_a_ent = transform_system.get_entity(collider_a.transform_id);
				Uint_32 t_b_ent = transform_system.get_entity(collider_b.transform_id);
				Uint_32 rb_a = rigidbody2d_system.get_component_id(t_a_ent);
				Uint_32 rb_b = rigidbody2d_system.get_component_id(t_b_ent);

				if (rb_a)
				{
					Rigidbody2D& rb = rigidbody2d_system.get_component(rb_a);
					if (!rb.is_kinematic)
						transform_a.Translate(-(Vec3f)(contacts[_i].depth*0.5f));
				}
				if (rb_b)
				{
					Rigidbody2D& rb = rigidbody2d_system.get_component(rb_b);
					if (!rb.is_kinematic)
						transform_b.Translate((Vec3f)(contacts[_i].depth*0.5f));
				}
				/*	Float_32 _x = abs<Float_32>(transforms[tr_a].get_position().x);
					_x = abs<Float_32>(transforms[tr_b].get_position.x);
					continue;*/
					//LOG(LOGTYPE_GENERAL, std::to_string(contacts[_i].depth.length()));

			}

			// all consumed
			contacts.clear();
		}
	}


	void PhysicsModule::end()
	{
		rigidbody2d_system.end();
	}

}