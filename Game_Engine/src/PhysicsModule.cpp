#include "PhysicsModule.hpp"
#include "EntityManagementSystemModule.hpp"
#include "RendererOpenGL2D.hpp"
namespace PrEngine {

	ComponentSystem<Rigidbody2D> PhysicsModule::rigidbody2d_system(Max_rigidbody2d_count);
	ComponentSystem<Collider> PhysicsModule::collider_system(Max_collider_count);
	Vec2f PhysicsModule::gravity;

	PhysicsModule* physics_module = nullptr;

	std::vector<Contact> PhysicsModule::contacts;
	std::unordered_map<std::string, ContactManidfold> contact_map;
	PhysicsModule::PhysicsModule(std::string name, Int_32 priority) :Module(name, priority)
	{
		if(!physics_module)
			physics_module = this;

		gravity = { 0, -10.f };
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
				switch (col.type)
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
						transformed_points[_i] = transform.transformation*col.points[_i];
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


	Float_32 get_parallelity(Vec2f p0, Vec2f p1, Vec2f ref)
	{
		if ((p0 - p1).GetMagnitude() > EPSILON)
		{
			Vec2f dir = (p0 - p1).GetNormalized();
			Float_32 val = Dot(dir, ref);
			return 1.f - abs<Float_32>(val);
		}
		return 0;
	}


	void calculate_contact_mass(Vec2f p, Vec2f dir, Rigidbody2D& rb_A, Rigidbody2D& rb_B, Contact& contact)
	{
		contact.rA = p - rb_A.position;
		contact.rB = p - rb_B.position;
		float rn1 = Dot(contact.rA, dir);
		float rn2 = Dot(contact.rB, dir);
		Float_32 mass_inv_A = rb_A.is_kinematic ? 0 : rb_A.mass_inverse;
		Float_32 mass_inv_B = rb_B.is_kinematic ? 0 : rb_B.mass_inverse;
		Float_32 inertia_inv_A = rb_A.is_kinematic ? 0 : rb_A.inertia_inverse;
		Float_32 inertia_inv_B = rb_B.is_kinematic ? 0 : rb_B.inertia_inverse;

		contact.normal_mass = mass_inv_A + mass_inv_B;
		contact.normal_mass += inertia_inv_A * (Dot(contact.rA, contact.rA) - rn1 * rn1) + inertia_inv_B * (Dot(contact.rB, contact.rB) - rn2 * rn2);

		// and friction
		Vec2f tangent = Cross(1.f, dir);
		float rt1 = Dot(contact.rA, tangent);
		float rt2 = Dot(contact.rB, tangent);
		contact.tangent_mass = mass_inv_A + mass_inv_B;
		contact.tangent_mass += inertia_inv_A * (Dot(contact.rA, contact.rA) - rt1 * rt1) + inertia_inv_B * (Dot(contact.rB, contact.rB) - rt2 * rt2);
	}

	void PhysicsModule::update()
	{
		static Vec2f contact_p1;
		static Vec2f contact_p2;

		static Float_32 accumulator = 0.0f;
		Float_32 dt = 1 / 60.f;
		Float_32 frame_time = Time::Frame_time;
		if (frame_time > 0.1f)
			frame_time = 0.1f;
		accumulator += frame_time;

		// copy transform values;
		for (Uint_32 i = 1; i < rigidbody2d_system.new_id; i++)
		{
			Rigidbody2D& rb = rigidbody2d_system.get_component(i);
			Transform3D& tr = transform_system.get_component(rb.transform_id);
			rb.position = (Vec2f)tr.get_global_position();
			Quaternion rot = tr.get_global_rotation();
			rb.rotation = Quaternion::QuaternionToEuler(rot).z * 0.0174533f;

			// temp
			// add gravity
			if(!rb.is_kinematic)
				rb.velocity += gravity * dt;
		}


		while (accumulator >= dt)
		{
			// detect collision
			// calculate contact point
			// calculate contact point impulse
			for (Uint_32 i = 1; i < rigidbody2d_system.new_id; i++)
			{
				for (Uint_32 j = i+1; j < rigidbody2d_system.new_id; j++)
				{
					Rigidbody2D& rb_A = rigidbody2d_system.get_component(i);
					Rigidbody2D& rb_B = rigidbody2d_system.get_component(j);
					Collider& col_a = collider_system.get_component(rb_A.collider_id);
					Collider& col_b = collider_system.get_component(rb_B.collider_id);
					
					std::vector<SimplexPoint> simplex;
					Bool_8 did_intersect = intersect_GJK(col_a, col_b, simplex);

					std::vector<Vec2f> simplex_points;
					if (did_intersect)
					{
						SimplexPoint sp_closest;

						std::vector<Vec2f> shape_a_points;
						std::vector<Vec2f> shape_b_points;

						for (SimplexPoint sp : simplex)
						{
							shape_a_points.push_back(sp.support_point_1);
							shape_b_points.push_back(sp.support_point_2);
						}

						Vec2f penetration = do_EPA(col_a, col_b, simplex);
						/*Contact contact;
						contact.normal = penetration.GetNormalized();
						contact.depth = penetration.GetMagnitude();
						contact.collider_a = i;
						contact.collider_b = j;*/

						Vec2f a_p0;
						Vec2f a_p1;
						Float_32 _max = 0;
						Vec2f contact_normal = penetration.GetNormalized();
						Vec2f points[4];
						Uint_32 point_count = 0;
						Uint_32 third_point_ind = 0;

						if (contact_normal.GetSqrdMagnitude() > EPSILON)
						{
							for (Uint_32 _k = 0; _k < shape_a_points.size(); _k++)
							{
								Vec3f p0 = shape_a_points[_k];
								Vec3f p1 = shape_a_points[(_k + 1) % shape_a_points.size()];
								Vec3f p2 = shape_a_points[(_k + 2) % shape_a_points.size()];
								Vec3f _dir = (p0 - p1).Normalize();
								Vec3f _crossed = Cross(_dir, (Vec3f)contact_normal);
								Vec3f dir_to_surface = Cross(_crossed, _dir);
								Float_32 p2_proj = Dot(dir_to_surface, p2 - p1);
								if (p2_proj <= EPSILON)
								{
									Float_32 p = get_parallelity(p0, p1, contact_normal);
									if (p > _max)
									{
										a_p0 = shape_a_points[_k];
										a_p1 = shape_a_points[(_k + 1) % shape_a_points.size()];
										_max = p;
									}
								}
							}

							

							if (_max >= 0.99f)
							{
								points[0] = a_p0;
								points[1] = a_p1;
								point_count += 2;
							}
							else
							{

								if ((a_p0 - a_p1).GetSqrdMagnitude() <= EPSILON)	// only one unique point
								{
									points[0] = shape_a_points[0];
								}
								else
								{
									Float_32 proj_p0 = Dot(contact_normal, a_p0);
									Float_32 proj_p1 = Dot(contact_normal, a_p1);
									if (proj_p0 > proj_p1)
									{
										points[0] = a_p0;
									}
									else
									{
										points[0] = a_p1;
									}
								}

								points[1]= { 0, 0 };
								point_count++;
								third_point_ind = 0;
							}



							Vec2f b_p0;
							Vec2f b_p1;
							_max = 0;
							for (Uint_32 _k = 0; _k < shape_b_points.size(); _k++)
							{
								Vec3f p0 = shape_b_points[_k];
								Vec3f p1 = shape_b_points[(_k + 1) % shape_b_points.size()];
								Vec3f p2 = shape_b_points[(_k + 2) % shape_b_points.size()];
								Vec3f _dir = (p0 - p1).Normalize();
								Vec3f _crossed = Cross(_dir, -(Vec3f)contact_normal);
								Vec3f dir_to_surface = Cross(_crossed, _dir);
								Float_32 p2_proj = Dot(dir_to_surface, p2 - p1);
								if (p2_proj <= EPSILON)
								{
									Float_32 p = get_parallelity(shape_b_points[_k], shape_b_points[(_k + 1) % shape_b_points.size()], contact_normal);
									if (p > _max)
									{
										b_p0 = shape_b_points[_k];
										b_p1 = shape_b_points[(_k + 1) % shape_b_points.size()];
										_max = p;
									}
								}
							}
							//if (_max >= 1.f - EPSILON)
							if (_max >= 0.99f)
							{
								points[2] = b_p0;
								points[3] = b_p1;
								point_count += 2;
							}
							else
							{
								if ((b_p0 - b_p1).GetSqrdMagnitude() <= EPSILON)	// only one unique point
								{
									points[2] = shape_b_points[0];
								}
								else
								{
									Float_32 proj_p0 = Dot(-contact_normal, b_p0);
									Float_32 proj_p1 = Dot(-contact_normal, b_p1);
									if (proj_p0 > proj_p1)
									{
										points[2] = b_p0;
									}
									else
									{
										points[2] = b_p1;
									}
								}
								points[3] = { 0, 0 };
								point_count++;
								third_point_ind = 2;
							}
						}
						/*Uint_32 tr_id_a = collider_system.get_component(contact.collider_a).transform_id;
						Transform3D& tr_a = transform_system.get_component(tr_id_a);
						Uint_32 tr_id_b = collider_system.get_component(contact.collider_b).transform_id;
						Transform3D& tr_b = transform_system.get_component(tr_id_b);*/

						//Uint_32 ent_a = collider_system.get_entity(i);
						//Uint_32 ent_b = collider_system.get_entity(j);
						//Uint_32 rb_a_id = rigidbody2d_system.get_component_id(ent_a);
						//Uint_32 rb_b_id = rigidbody2d_system.get_component_id(ent_b);

						//Vec2f vel_a;
						//Vec2f vel_b;
						////Vec2f pos_a = rb_A.position;// transform_system.get_component(t_id_i).get_global_position();
						////Vec2f pos_b = rb_B.position;// transform_system.get_component(t_id_j).get_global_position();


						//if (rb_a_id)
						//{
						//	Rigidbody2D& rb_a = rigidbody2d_system.get_component(rb_a_id);
						//	vel_a = rb_a.velocity;
						//}

						//if (rb_b_id)
						//{
						//	Rigidbody2D& rb_b = rigidbody2d_system.get_component(rb_b_id);
						//	vel_b = rb_b.velocity;
						//}


						



						if (point_count == 4)
						{

							/*if (abs<Float_32>(rb_A.angular_velocity) < 0.5f)
								rb_A.angular_velocity *= 0.5f;
							if (abs<Float_32>(rb_B.angular_velocity) < 0.5f)
								rb_B.angular_velocity *= 0.5;*/

							Vec2f p0 = points[0];
							Vec2f p1 = points[1];
							Vec2f p2 = points[2];
							Vec2f p3 = points[3];

							Vec2f proj_dir = (p1 - p0);
							Vec2f proj_dir_norm = proj_dir.GetNormalized();
							Float_32 proj_len = proj_dir.GetMagnitude();

							Float_32 p2_proj = Dot(proj_dir_norm, p2 - p0);
							Float_32 p3_proj = Dot(proj_dir_norm, p3 - p0);

							// p3 must always have largest projection along proj_dir
							if (p2_proj > p3_proj)
							{
								std::swap(p2, p3);
								std::swap(p2_proj, p3_proj);
							}

							

							// figure out impact point from overlap
							if (p2_proj < 0 && (p3_proj >= 0 && p3_proj <= proj_len))
							{

								{
									Contact c1{ p0, penetration.GetNormalized(), { 0,0 }, { 0,0 }, i, j, penetration.GetMagnitude()};
									calculate_contact_mass(p0, contact_normal, rb_A, rb_B, c1);

									contacts.push_back(c1);
								}

								{
									Contact c1{ p3, penetration.GetNormalized(),{0,0},{0,0}, i,j, penetration.GetMagnitude()};
									calculate_contact_mass(p3, contact_normal, rb_A, rb_B, c1);
									contacts.push_back(c1);
								}
								//impact_point = (p0 + p3) / 2;
							}
							else if ((p2_proj >= 0 && p2_proj <= proj_len) && p3_proj > proj_len)
							{
								{
									Contact c1{ p2, penetration.GetNormalized(),{0,0},{0,0},i,j, penetration.GetMagnitude()};
									calculate_contact_mass(p2, contact_normal, rb_A, rb_B,c1);
									contacts.push_back(c1);
								}

								{
									Contact c1{ p1, penetration.GetNormalized(),{0,0},{0,0},i,j, penetration.GetMagnitude()};
									calculate_contact_mass(p1, contact_normal, rb_A, rb_B, c1);
									contacts.push_back(c1);
								}

								//impact_point = (p1 + p2) / 2;
							}
							else if (p2_proj <0 && p3_proj > proj_len)
							{
								{

									Contact c1{ p0, penetration.GetNormalized(),{0,0},{0,0},i,j, penetration.GetMagnitude()};
									calculate_contact_mass(p0, contact_normal, rb_A, rb_B, c1);
									contacts.push_back(c1);

								}

								{

									Contact c1{ p1, penetration.GetNormalized(),{0,0},{0,0},i,j, penetration.GetMagnitude()};
									calculate_contact_mass(p1, contact_normal, rb_A, rb_B, c1);
									contacts.push_back(c1);
								}

								//impact_point = (p0 + p1) / 2;
							}
							else
							{
								{
									Contact c1{ p2, penetration.GetNormalized(),{0,0},{0,0},i,j, penetration.GetMagnitude()};
									calculate_contact_mass(p2, contact_normal, rb_A, rb_B, c1);
									contacts.push_back(c1);
								}

								{
									Contact c1{ p3, penetration.GetNormalized(),{0,0},{0,0},i,j, penetration.GetMagnitude()};
									calculate_contact_mass(p3, contact_normal, rb_A, rb_B, c1);
									contacts.push_back(c1);
								}

								//impact_point = (p2 + p3) / 2;
							}

						}
						else if (point_count == 3)
						{
							//Vec2f impact_point;
							Vec2f p = points[third_point_ind];

							Contact c1{ p, penetration.GetNormalized(),{0,0},{0,0},i,j, penetration.GetMagnitude()};
							calculate_contact_mass(p, contact_normal, rb_A, rb_B, c1);

							contacts.push_back(c1);
							//impact_point = contact.points[contact.third_point_ind].global_position;
						}
						else if (point_count == 2)
						{
							Vec2f p = (points[0] + points[2]) / 2.f;
							Contact c1{ p, penetration.GetNormalized(),{0,0},{0,0},i,j, penetration.GetMagnitude()};
							calculate_contact_mass(p, contact_normal, rb_A, rb_B, c1);
							contacts.push_back(c1);
							//impact_point = (contact.points[0].global_position + contact.points[2].global_position) / 2;
						}
						else if (point_count == 1)
						{
							Vec2f p = points[0];
							Contact c1{ p, penetration.GetNormalized(),{0,0},{0,0},i,j, penetration.GetMagnitude()};
							calculate_contact_mass(p, contact_normal, rb_A, rb_B, c1);
							contacts.push_back(c1);
							//impact_points.push_back(contact.points[0]);
							//impact_point = contact.points[0].global_position;
							//LOG(LOGTYPE_ERROR, "Shouldn't be here!");
						}

						Vec4f red_color{ 1.0, 0, 0, 1.0 };
						Vec4f cyan_color{ 0.0, 1, 1, 1.0 };
						Rect<Float_32> a = points_to_rect(collider_system.get_component(i).points);//, transforms[tr_a].transformation);
						Rect<Float_32> b = points_to_rect(collider_system.get_component(j).points);//, transforms[tr_b].transformation);
					}
				}
			}


			/*
			// do collision check and find contact information
			for (Uint_32 i = 0; i < collider_system.new_id; i++)
			{
				if (collider_system.get_entity(i))
				{
					Uint_32 t_id_i = collider_system.get_component(i).transform_id;
					//if (!t_id_i)continue;

					for (Uint_32 j = i + 1; j < collider_system.new_id; j++)
					{
						{
							Uint_32 t_id_j = collider_system.get_component(j).transform_id;
							//if (!t_id_j)continue;
						
							Collider& col_a = collider_system.get_component(i);
							Collider& col_b = collider_system.get_component(j);
							//Transform3D& tr_a = transform_system.get_component(col_a.transform_id);
							//Transform3D& tr_b = transform_system.get_component(col_b.transform_id);

							std::vector<SimplexPoint> simplex;
							Bool_8 did_intersect = intersect_GJK(collider_system.get_component(i), collider_system.get_component(j), simplex);

							std::vector<Vec2f> simplex_points;


							if (did_intersect)
							{
								SimplexPoint sp_closest;

								std::vector<Vec2f> shape_a_points;
								std::vector<Vec2f> shape_b_points;

								for (SimplexPoint sp : simplex)
								{
									shape_a_points.push_back(sp.support_point_1);
									shape_b_points.push_back(sp.support_point_2);
								}

								ContactManidfold& contact_manifold = do_EPA(i, j, simplex);

								Vec2f a_p0;
								Vec2f a_p1;
								Float_32 _max = 0;
								Vec2f ref = contact_manifold.normal.GetNormalized();
								if (ref.GetMagnitude() > EPSILON)
								{
									for (Uint_32 _k = 0; _k < shape_a_points.size(); _k++)
									{
										Vec3f p0 = shape_a_points[_k];
										Vec3f p1 = shape_a_points[(_k + 1) % shape_a_points.size()];
										Vec3f p2 = shape_a_points[(_k + 2) % shape_a_points.size()];
										Vec3f _dir = (p0 - p1).Normalize();
										Vec3f _crossed = Cross(_dir, (Vec3f)ref);
										Vec3f dir_to_surface = Cross(_crossed, _dir);
										Float_32 p2_proj = Dot(dir_to_surface, p2 - p1);
										if (p2_proj <= EPSILON)
										{
											Float_32 p = get_parallelity(p0, p1, ref);
											if (p > _max)
											{
												a_p0 = shape_a_points[_k];
												a_p1 = shape_a_points[(_k + 1) % shape_a_points.size()];
												_max = p;
											}
										}
									}

									
									if (_max >= 0.99f)
									{
										contact_manifold.points[0].global_position = a_p0;
										contact_manifold.points[0].local_position = tr_a.transformation.GetInverse() * a_p0;

										contact_manifold.points[1].global_position = a_p1;
										contact_manifold.points[1].local_position = tr_a.transformation.GetInverse() * a_p1;

										//renderer->draw_point(a_p0);
										//renderer->draw_point(a_p1);
										contact_manifold.point_count += 2;
									}
									else
									{

										if ((a_p0 - a_p1).GetMagnitude() <= EPSILON)	// only one unique point
										{
											contact_manifold.points[0].global_position = shape_a_points[0];
											contact_manifold.points[0].local_position = tr_a.transformation.GetInverse() * shape_a_points[0];

											//renderer->draw_point(a_p1);
										}
										else
										{
											Float_32 proj_p0 = Dot(ref, a_p0);
											Float_32 proj_p1 = Dot(ref, a_p1);
											if (proj_p0 > proj_p1)
											{
												contact_manifold.points[0].global_position = a_p0;
												contact_manifold.points[0].local_position = tr_a.transformation.GetInverse() * a_p0;

												//renderer->draw_point(a_p0);
											}
											else
											{
												contact_manifold.points[0].global_position = a_p1;
												contact_manifold.points[0].local_position = tr_a.transformation.GetInverse() * a_p1;

												//renderer->draw_point(a_p1);
											}
										}

										contact_manifold.points[1].global_position = { 0, 0 };
										contact_manifold.points[1].local_position = { 0, 0 };
										contact_manifold.point_count++;

									}



									Vec2f b_p0;
									Vec2f b_p1;
									_max = 0;
									for (Uint_32 _k = 0; _k < shape_b_points.size(); _k++)
									{
										Vec3f p0 = shape_b_points[_k];
										Vec3f p1 = shape_b_points[(_k + 1) % shape_b_points.size()];
										Vec3f p2 = shape_b_points[(_k + 2) % shape_b_points.size()];
										Vec3f _dir = (p0 - p1).Normalize();
										Vec3f _crossed = Cross(_dir, -(Vec3f)ref);
										Vec3f dir_to_surface = Cross(_crossed, _dir);
										Float_32 p2_proj = Dot(dir_to_surface, p2 - p1);
										if (p2_proj <= EPSILON)
										{
											Float_32 p = get_parallelity(shape_b_points[_k], shape_b_points[(_k + 1) % shape_b_points.size()], ref);
											if (p > _max)
											{
												b_p0 = shape_b_points[_k];
												b_p1 = shape_b_points[(_k + 1) % shape_b_points.size()];
												_max = p;
											}
										}
									}
									//if (_max >= 1.f - EPSILON)
									if (_max >= 0.99f)
									{
										contact_manifold.points[2].global_position = b_p0;
										contact_manifold.points[2].local_position = tr_b.transformation.GetInverse() * b_p0;

										contact_manifold.points[3].global_position = b_p1;
										contact_manifold.points[3].local_position = tr_b.transformation.GetInverse() * b_p1;

										//renderer->draw_point(b_p0);
										//renderer->draw_point(b_p1);
										contact_manifold.point_count += 2;
									}
									else
									{
										if ((b_p0 - b_p1).GetMagnitude() <= EPSILON)	// only one unique point
										{
											contact_manifold.points[2].global_position = shape_b_points[0];
											contact_manifold.points[2].local_position = tr_b.transformation.GetInverse() * shape_b_points[0];
											
										//	renderer->draw_point(a_p1);
										}
										else
										{
											Float_32 proj_p0 = Dot(-ref, b_p0);
											Float_32 proj_p1 = Dot(-ref, b_p1);
											if (proj_p0 > proj_p1)
											{
												contact_manifold.points[2].global_position = b_p0;
												contact_manifold.points[2].local_position = tr_b.transformation.GetInverse() * b_p0;

											//	renderer->draw_point(b_p0);
											}
											else
											{
												contact_manifold.points[2].global_position = b_p1;
												contact_manifold.points[2].local_position = tr_b.transformation.GetInverse() * b_p1;

											//	renderer->draw_point(b_p1);
											}
										}
										contact_manifold.points[3].global_position = { 0, 0 };
										contact_manifold.points[3].local_position = { 0, 0 };
										contact_manifold.point_count++;
									}
								}
								Uint_32 tr_id_a = collider_system.get_component(contact_manifold.collider_a).transform_id;
								Transform3D& tr_a = transform_system.get_component(tr_id_a);
								Uint_32 tr_id_b = collider_system.get_component(contact_manifold.collider_b).transform_id;
								Transform3D& tr_b = transform_system.get_component(tr_id_b);

								Uint_32 ent_a = collider_system.get_entity(i);
								Uint_32 ent_b = collider_system.get_entity(j);
								Uint_32 rb_a_id = rigidbody2d_system.get_component_id(ent_a);
								Uint_32 rb_b_id = rigidbody2d_system.get_component_id(ent_b);

								Vec2f vel_a;
								Vec2f vel_b;
								Vec2f pos_a = transform_system.get_component(t_id_i).get_global_position();
								Vec2f pos_b = transform_system.get_component(t_id_j).get_global_position();


								if (rb_a_id)
								{
									Rigidbody2D& rb_a = rigidbody2d_system.get_component(rb_a_id);
									vel_a = rb_a.velocity;
								}

								if (rb_b_id)
								{
									Rigidbody2D& rb_b = rigidbody2d_system.get_component(rb_b_id);
									vel_b = rb_b.velocity;
								}
								physics_module->contacts.push_back(contact_manifold);

								Vec4f red_color{ 1.0, 0, 0, 1.0 };
								Vec4f cyan_color{ 0.0, 1, 1, 1.0 };
								Rect<Float_32> a = points_to_rect(collider_system.get_component(i).points);//, transforms[tr_a].transformation);
								Rect<Float_32> b = points_to_rect(collider_system.get_component(j).points);//, transforms[tr_b].transformation);

								//renderer->draw_rect_with_transform(a, red_color, transform_system.get_component(t_id_i).transformation);
								//renderer->draw_rect_with_transform(b, red_color, transform_system.get_component(t_id_j).transformation);
							}

						}
					}

				}
			}

			*/

			


			
			for (int iter = 0; iter < 10; iter++)
			{			// apply impulses
				for (int _i = 0; _i < contacts.size(); _i++)
				{

					Contact& contact = contacts[_i];


					Rigidbody2D& rb_A = rigidbody2d_system.get_component(contact.Rigidbody2d_a);
					Rigidbody2D& rb_B = rigidbody2d_system.get_component(contact.Rigidbody2d_b);
					Vec4f cyan_color{ 0.0, 1, 1, 1.0 };

					if (contact.depth > EPSILON)
					{
						renderer->draw_point(contact.position);
						/*	Vec3f r_1 = impact_point - transform_a.get_global_position();
							Vec2f _r1_n = Cross(Cross(r_1, (Vec3f)contact_normal), r_1);
							Vec3f r_2 = impact_point - transform_b.get_global_position();
							Vec2f _r2_n = Cross(Cross(r_2, (Vec3f)contact_normal), r_2);
							Vec2f v1 = _r1_n * (1.f / in_a);
							Vec2f v2 = _r2_n * (1.f / in_b);
							Vec2f v3 = v1 + v2;
							Float_32 rot_imp_comp = Dot(v3, contact_normal);
							Float_32 div = (1.f / ma + 1.f / mb) + rot_imp_comp;
							Vec2f point_vel_a = Cross(Vec3f{ 0,0,rb_A.angular_velocity }, (Vec3f)(impact_point - transform_a.get_global_position())) + rb_A.velocity;
							Vec2f point_vel_b = Cross(Vec3f{ 0,0,rb_B.angular_velocity }, (Vec3f)(impact_point - transform_b.get_global_position())) + rb_B.velocity;
							Float_32 contact_closing_velocity = Dot((point_vel_a - point_vel_b), contact.normal.GetNormalized());
							Float_32 Impulse = contact_closing_velocity * (1 + cor) / div;
							Impulse = abs<Float_32>(Impulse);*/
						Vec2f point_vel_a = rb_A.velocity + Cross(rb_A.angular_velocity, contact.rA);
						Vec2f point_vel_b = rb_B.velocity + Cross(rb_B.angular_velocity, contact.rB);
						Vec2f rel_vel = point_vel_b - point_vel_a;
						Float_32 cor = 0.f;// (rb_A.coefficient_of_restitution * rb_B.coefficient_of_restitution);
						Float_32 static_friction = rb_A.static_friction * rb_B.static_friction;
						Float_32 dynamic_friction = rb_A.dynamic_friction * rb_B.dynamic_friction;

						Float_32 bias_fact =  0.2f;
						Float_32 bias = -(contact.depth - 0.01f) * bias_fact *(1 / dt);
						
						Float_32 numerator = -(1 + cor)*(Dot(rel_vel, contact.normal)+bias);
						Float_32 normal_impulse = numerator / contact.normal_mass;
						normal_impulse = Max<Float_32>(normal_impulse, 0);

						renderer->draw_ray(contact.position, contact.normal, contact.depth, { 1,1,1,1 });

						rb_A.add_impulse(-contact.normal * normal_impulse);
						rb_B.add_impulse(contact.normal * normal_impulse);

						rb_A.add_impulse_at_point(-contact.normal * normal_impulse, contact.position);
						rb_B.add_impulse_at_point(contact.normal * normal_impulse, contact.position);


						// apply friction
						Vec2f tangent = { contact.normal.y, -contact.normal.x };

						numerator = -Dot(rel_vel, tangent);
						Float_32 tangent_impulse = numerator / contact.tangent_mass;
						Float_32 max_tangent_impulse = normal_impulse * static_friction;
						tangent_impulse = clamp<Float_32>(tangent_impulse, -max_tangent_impulse, max_tangent_impulse);

						renderer->draw_ray(contact.position, contact.normal, contact.depth, { 0,1,0,1 });

						rb_A.add_impulse(-tangent * tangent_impulse);
						rb_B.add_impulse( tangent * tangent_impulse);

						rb_A.add_impulse_at_point(- tangent * tangent_impulse, contact.position);
						rb_B.add_impulse_at_point( tangent * tangent_impulse, contact.position);
					}
				}
			}

			// all consumed
			contacts.clear();

			// update rigidbodies
			rigidbody2d_system.update(dt);
			accumulator -= dt;

			
		}

		// copy values back to transform;
		for (Uint_32 i = 1; i < rigidbody2d_system.new_id; i++)
		{
			Rigidbody2D& rb = rigidbody2d_system.get_component(i);
			Transform3D& tr = transform_system.get_component(rb.transform_id);
			tr.set_global_position(rb.position.x, rb.position.y, 0);
			tr.set_global_rotation(0, 0, rb.rotation*57.2958f);
		}
	}


	void PhysicsModule::end()
	{
		rigidbody2d_system.end();
	}

}