/*
 * EntityGenerator.cpp
 *
 *  Created on: Sep 15, 2020
 *      Author: Aniruddha Prithul
 */

#include "EntityGenerator.hpp"
#include "PhysicsModule.hpp"
#include <mutex>
#include <thread>

namespace PrEngine{

	std::mutex batched_texture_id_mutex;
	std::vector<Uint_32> EntityGenerator::batched_texture_ids;

	std::unordered_map<int, int> EntityGenerator::transform_id_mapping;

	EntityGenerator::EntityGenerator()
	{
		transform_id_mapping[0] = 0;
	}

	Uint_32 EntityGenerator::make_camera_orthographic(Float_32 width, Float_32 height, Float_32 _near, Float_32 _far)
	{
		//std::string entity_name = "Camera";
		Uint_32 entity = entity_management_system->make_entity("Camera");
		
		Uint_32 transform_id = transform_system.make(entity);
		transform_system.get_component(transform_id).set_local_position( 0.f, 0.f, -6.f );
		
		/*Uint_32 id_camera = entity_management_system->make_camera_comp(entity);
		cameras[id_camera].set_orthographic(-(width / 2.f), (width / 2.f), -(height / 2.f), (height / 2.f), _near, _far);
		cameras[id_camera].id_transform = id_transform;*/

		Uint_32 id_camera = camera_system.make(entity);
		if (id_camera)
		{
			Camera& camera = camera_system.get_component(id_camera);
			camera.set_orthographic(-(width / 2.f), (width / 2.f), -(height / 2.f), (height / 2.f), _near, _far);
			camera.id_transform = transform_id;
		}

		//auto text = camera_ent->to_string();
		//write_to_file(text, "scene.graph", false, true);
		return entity;
	}

	Uint_32 EntityGenerator::make_sprite(const std::string& material_path, Point3d position, RenderTag render_tag)
	{
		Uint_32 entity = entity_management_system->make_entity("Sprite");
		if (entity)
		{
			Uint_32 transform_id = transform_system.make(entity);// entity_management_system->make_transform_comp(entity);
			transform_system.get_component(transform_id).set_local_position(position);

			make_sprite(entity, 1.0f, render_tag, 0, transform_id, material_path);
		}
		
		return entity;
	}
	
	Uint_32 EntityGenerator::make_animated_sprite(Point3d position, const std::string& animation_path, const std::string& material_path)
	{
		Uint_32 entity = entity_management_system->make_entity("Animated Sprite");
		
		Uint_32 transform_id = transform_system.make(entity);
		transform_system.get_component(transform_id).set_local_position(position);

		Uint_32 animator_id = animator_system.make(entity);// entity_management_system->make_animator_comp(entity);
		if (animator_id)
		{
			Animator& animator = animator_system.get_component(animator_id);
			animator.cur_anim_ind = 0;
			animator.anim_transform_update_flags[ANIM_TRANSLATE] = false;
			animator.anim_transform_update_flags[ANIM_ROTATE] = false;
			animator.anim_transform_update_flags[ANIM_SCALE] = false;
			animator.id_transform = transform_id;
			Uint_32 a_id = 0;
			if (a_id = Animator::load_animation(animation_path))
			{
				animator.add_animation(a_id);
			}
			else
				LOG(LOGTYPE_ERROR, "Couldn't add animation");
			make_sprite(entity, 1.0f, RENDER_DYNAMIC, animator_id, transform_id, material_path);
		}
		return entity;
	}

	Uint_32 EntityGenerator::make_light_entity()
	{
		Uint_32 entity = entity_management_system->make_entity("Light");

		//Uint_32 id_transform = entity_management_system->make_transform_comp(entity);
		//get_transform(id_transform).set_local_rotation(0, 0, 90);

		//Uint_32 id_dir_light = entity_management_system->make_directional_light_comp(entity);
		//directional_lights[id_dir_light].specular = 0.5f;
		//directional_lights[id_dir_light].ambient = 0.3f;
		//directional_lights[id_dir_light].id_transform = id_transform;

		////auto text = light_ent->to_string();
		////write_to_file(text, "scene.graph", false, true);

		return entity;
	}

	Uint_32 EntityGenerator::make_sprite(Uint_32 entity, Float_32 import_scale, RenderTag render_tag, Uint_32 animator_id, Uint_32 transform_id, const std::string& material_name)
	{
		
		//Uint_32 graphic_id = entity_management_system->make_graphic_comp(entity);
		Uint_32 graphic_id = graphics_system.make(entity);
		if (graphic_id)
		{
			Graphic& graphic = graphics_system.get_component(graphic_id);
			RenderTag _render_tag = render_tag;
			RenderTag future_render_tag = render_tag;
			Graphic::editor_data[graphic_id] = { import_scale, future_render_tag };


			//graphics[graphic_id].tag = _render_tag;
			graphic.tag = _render_tag;

			//graphics[id_graphic].future_tag = future_render_tag;

			Bool_8 create_gl_texture = _render_tag == RENDER_UNTAGGED;

			Uint_32 mat_id = Material::load_material(material_name, create_gl_texture);
			LOG(LOGTYPE_GENERAL, "mat created");
			/*graphics[graphic_id].element.material = mat_id;

			assert(transform_id);
			graphics[graphic_id].transform_id = transform_id;
			graphics[graphic_id].animator_id = animator_id;*/
			graphic.element.material = mat_id;
			graphic.transform_id = transform_id;
			graphic.animator_id = animator_id;

			switch (_render_tag)
			{
			case RENDER_UNTAGGED:
			{
				//LOG(LOGTYPE_GENERAL, "creating sprite graphics");
				renderer->generate_sprite_graphics(graphic_id);
			}
			break;
			case RENDER_STATIC:
			case RENDER_DYNAMIC:
				//static_batched_graphic_ids.push_back(graphic_id);
				//renderer->generate_batched_sprite_graphics(id_graphic);
				//break;
			
				//renderer->generate_sprite_graphics(id_graphic);
				Material* material = Material::get_material(mat_id);
				for (Uint_32 _i = 0; _i < 8; _i++)
				{
					if (material->diffuse_textures[_i] != -1)
					{
						batched_texture_id_mutex.lock();
							batched_texture_ids.push_back(material->diffuse_textures[0]);
						batched_texture_id_mutex.unlock();
					}
				}
				break;
			}
			LOG(LOGTYPE_GENERAL, "added to batch list");
		}
		return graphic_id;
	}

#ifdef  EDITOR_MODE


	void load_all_resrources()
	{

	}
#endif //  EDITOR_MODE


	Uint_32 EntityGenerator::duplicate_entity(Uint_32 entity)
	{
		std::string name = entity_management_system->entity_names[entity] + " (Copy)";
		Uint_32 new_entity = entity_management_system->make_entity(name);
		if (new_entity)
		{
			Uint_32 transform_id = 0;
			Uint_32 graphic_id = 0;
			Uint_32 collider_id = 0;

			// duplicate transform
			Uint_32 other_transform_id = transform_system.get_component_id(entity);
			if (other_transform_id)
			{
				Transform3D& other_transform = transform_system.get_component(other_transform_id);
				transform_id = transform_system.make(new_entity);
				if (transform_id)
				{
					Transform3D transform_copy(other_transform);
					Transform3D& transform = transform_system.get_component(transform_id);
					transform_copy.children = transform.children;

					transform = transform_copy;
					//transform = transform_system.get_component(transform_system.get_component_id(entity));
				}
			}

			// duplicate grpahic
			Uint_32 other_graphic_id = graphics_system.get_component_id(entity);
			if (other_graphic_id)
			{
				Graphic& other_graphic = graphics_system.get_component(other_graphic_id);
				std::string& mat_name = Material::material_names[other_graphic.element.material];
				graphic_id = make_sprite(new_entity, Graphic::editor_data[other_graphic_id].scale, (RenderTag)Graphic::editor_data[other_graphic_id].future_tag, 0, transform_id, mat_name);
			}


			// duplicate Collider
			Uint_32 other_collider_id = PhysicsModule::collider_system.get_component_id(entity);
			if (other_collider_id)
			{
				Collider& other_collider = PhysicsModule::collider_system.get_component(other_collider_id);
				collider_id = PhysicsModule::collider_system.make(new_entity);
				if (collider_id)
				{
					Collider collider(other_collider);
					collider.graphic_id = graphic_id;
					collider.transform_id = transform_id;
					PhysicsModule::collider_system.get_component(collider_id) = collider;
				}
			}


			// duplicate rigidbody2d
			Uint_32 other_rigidbody2d_id = PhysicsModule::rigidbody2d_system.get_component_id(entity);
			if (other_rigidbody2d_id)
			{
				Rigidbody2D& other_rigidbody2d = PhysicsModule::rigidbody2d_system.get_component(other_rigidbody2d_id);
				Uint_32 rigidbody2d_id = PhysicsModule::rigidbody2d_system.make(new_entity);
				if (rigidbody2d_id)
				{
					Rigidbody2D rigidbody2d(other_rigidbody2d);
					rigidbody2d.transform_id = transform_id;
					rigidbody2d.collider_id = collider_id;
					PhysicsModule::rigidbody2d_system.get_component(rigidbody2d_id) = rigidbody2d;
				}
			}


		}

		return new_entity;

	}


	void EntityGenerator::generate_entity_in_thraed(std::string entity_string)
	{
		trim(entity_string);
		std::stringstream ent(entity_string);
		std::string comp_str;
		std::string entity_name = "";
		std::getline(ent, entity_name);

		if (entity_name == "Materials")
		{

			while (std::getline(ent, comp_str))	// comp str is actually the name of the many materials in this case
			{
				std::string material_path = comp_str;
				trim(material_path);
				Material::load_material(material_path, false);
			}
		}
		else
		{
			//Entity* entity = EntityManagementSystem::entity_management_system->make_entity(entity_name);
			Uint_32 entity = entity_management_system->make_entity(entity_name);
			if (entity)
			{
				Uint_32 transform_id = 0;
				Uint_32 graphic_id = 0;
				Uint_32 animator_id = 0;
				while (std::getline(ent, comp_str)) // get a componenet in the entity
				{
					std::stringstream comp(comp_str);
					std::string token;
					std::vector<std::string> tokens;
					while (std::getline(comp, token, ','))
					{
						tokens.push_back(token);
					}

					if (tokens.size() > 0)
					{
						Int_32 comp_type = std::stoi(tokens[0], nullptr, 10);
						LOG(LOGTYPE_GENERAL, "COMP TYPE: ", std::to_string(comp_type));
						switch (comp_type)
						{
							/*case COMP_SPRITE:
							{

								Sprite* sprite = new Sprite(std::stoi(tokens[1]));
								sprite->add_to_renderer(renderer);
								entity->add_componenet(sprite);
								break;
							}*/
						case COMP_COLLIDER:
						{
							LOG(LOGTYPE_WARNING, "will make collider");
							// to be moved
							//Uint_32 collider_id = entity_management_system->make_collider_comp(entity);
							//Collider& col = colliders[collider_id];

							Uint_32 collider_id = PhysicsModule::collider_system.make(entity);
							Collider& col = PhysicsModule::collider_system.get_component(collider_id);

							col.graphic_id = graphic_id;
							col.transform_id = transform_id;
							col.type = (Shape2DTypes)std::atoi(tokens[1].c_str());
							for (int _i = 2; _i < tokens.size() - 1; _i += 2)
							{
								Vec2f p{ std::strtof(tokens[_i].c_str(), nullptr), std::strtof(tokens[_i + 1].c_str(), nullptr) };
								col.points[(_i / 2) - 1] = p;
								col.point_count++;
							}

							LOG(LOGTYPE_WARNING, "made collider");

						}
						break;
						case COMP_ANIMATOR:
						{

							animator_id = animator_system.make(entity);
							if (animator_id)
							{
								Animator& animator = animator_system.get_component(animator_id);
								animator.cur_anim_ind = std::stod(tokens[1]);
								animator.anim_transform_update_flags[0] = std::stod(tokens[2]);
								animator.anim_transform_update_flags[1] = std::stod(tokens[3]);
								animator.anim_transform_update_flags[2] = std::stod(tokens[4]);

								assert(transform_id);
								animator.id_transform = transform_id;

								for (int t_i = 5; t_i < tokens.size(); t_i++)
								{
									Uint_32 a_id = 0;
									trim(tokens[t_i]);
									if (a_id = Animator::load_animation(tokens[t_i]))
									{
										animator.add_animation(a_id);
										Animation& animation = animator.get_animation(a_id);

										for (Keyframe frame : animation.frames)
										{
											batched_texture_id_mutex.unlock();
											batched_texture_ids.push_back(frame.texture);
											batched_texture_id_mutex.unlock();

										}
									}
									else
										LOG(LOGTYPE_ERROR, "Couldn't create animator");
								}
							}
							/*
														animator_id = entity_management_system->make_animator_comp(entity);
														animators[animator_id].cur_anim_ind = std::stod(tokens[1]);
														animators[animator_id].anim_transform_update_flags[0] = std::stod(tokens[2]);
														animators[animator_id].anim_transform_update_flags[1] = std::stod(tokens[3]);
														animators[animator_id].anim_transform_update_flags[2] = std::stod(tokens[4]);*/

														//assert(transform_id);
														//	animators[animator_id].id_transform = transform_id;
														//	for (int t_i = 5; t_i < tokens.size(); t_i++)
														//	{
														//		Uint_32 a_id = 0;
														//		trim(tokens[t_i]);
														//		if (a_id = Animator::load_animation(tokens[t_i]))
														//		{
														//			animators[animator_id].add_animation(a_id);// Animator::animations_library.size() - 1;
														//			/*Uint_32 entity_2 = entity_management_system->make_entity();
														//			Uint_32 id_transform_2 = entity_management_system->make_transform_comp(entity_2);
														//			entity_management_system->set_parent_transform(id_transform, id_transform_2);
														//			id_transform = id_transform_2;*/
														//			
														//		}
														//		else
														//			LOG(LOGTYPE_ERROR, "Couldn't create animator");
														//	}
															//assert(id_graphic);
															//animators[id_animator].id_graphic = id_graphic;
															/*Animator* animator = new Animator();
															for (Int_32 i = 1; i < tokens.size(); i++)
															{
																animator->load_animation(tokens[i]);
															}
															entity->add_componenet(animator);*/
							break;
						}
						case COMP_CAMERA:	// since 2d engine, assume orthographic 
						{
							Float_32 left = std::stof(tokens[2]);
							Float_32 right = std::stof(tokens[3]);
							Float_32 bottom = std::stof(tokens[4]);
							Float_32 top = std::stof(tokens[5]);
							Float_32 _near = std::stof(tokens[6]);
							Float_32 _far = std::stof(tokens[7]);
							Float_32 _zoom = std::stof(tokens[8]);
							/*Uint_32 id_camera = entity_management_system->make_camera_comp(entity);
							cameras[id_camera].set_orthographic(left, right, bottom, top, _near, _far);
							assert(transform_id);
							cameras[id_camera].id_transform = transform_id;*/
							Uint_32 id_camera = camera_system.make(entity);
							Camera& camera = camera_system.get_component(id_camera);
							camera.set_orthographic(left, right, bottom, top, _near, _far);
							camera.zoom = _zoom;
							assert(transform_id);
							camera.id_transform = transform_id;


							/*if (std::stoi(tokens[1]) == ORTHOGRAPHIC)
							{
								Float_32 l = std::stof(tokens[2]);
								Float_32 r = std::stof(tokens[3]);
								Float_32 b = std::stof(tokens[4]);
								Float_32 t = std::stof(tokens[5]);
								Float_32 n = std::stof(tokens[6]);
								Float_32 f = std::stof(tokens[7]);

								Camera* camera = new Camera(l, r, b, t, n, f);
								entity->add_componenet(camera);
							}*/
							break;
						}
						case COMP_GRAPHICS:
						{
							Float_32 import_scale = (Float_32)std::atof(tokens[3].c_str());
							std::string material_path = tokens[1];
							trim(material_path);
							RenderTag render_tag = (RenderTag)std::atoi(tokens[2].c_str());
							assert(transform_id);

							//Comment for benchmarking purpose, Uncomment in build
#ifdef EDITOR_MODE
							if (render_tag == RENDER_STATIC) render_tag = RENDER_UNTAGGED;
							//render_tag = RENDER_UNTAGGED;
#endif
							if (animator_id)
								render_tag = RENDER_DYNAMIC;

							////////////////////////////
							//render_tag = RENDER_DYNAMIC;

							make_sprite(entity, import_scale, render_tag, animator_id, transform_id, material_path);


							break;
						}
						case COMP_LIGHT:
						{
							/*Uint_32 id_dir_light = entity_management_system->make_directional_light_comp(entity);
							directional_lights[id_dir_light].specular = 0.5f;
							directional_lights[id_dir_light].ambient = 0.3f;
							assert(transform_id);
							directional_lights[id_dir_light].id_transform = transform_id;*/

							/*DirectionalLight* light = new DirectionalLight(std::stof(tokens[1]), std::stof(tokens[2]));
							entity->add_componenet(light);*/
							break;
						}
						case COMP_TRANSFORM_3D:
						{
							transform_id = transform_system.make(entity);
							LOG(LOGTYPE_WARNING, std::to_string(transform_id));
							transform_id_mapping[std::stoi(tokens[12])] = transform_id;	//mapping for finding parents
							Transform3D& _transform = transform_system.get_component(transform_id);
							_transform.set_local_position(std::stof(tokens[1]), std::stof(tokens[2]), std::stof(tokens[3]));
							_transform.set_local_scale(std::stof(tokens[4]), std::stof(tokens[5]), std::stof(tokens[6]));

							Quaternion rot = { std::stof(tokens[7]), std::stof(tokens[8]), std::stof(tokens[9]), std::stof(tokens[10]) };
							_transform.set_local_rotation(rot);
							Uint32 parent_transform_id = transform_id_mapping[std::stoi(tokens[11])];
							if (parent_transform_id)
								entity_management_system->set_parent_transform(parent_transform_id, transform_id);
							//transforms[transform_id].update_transformation();
							break;
						}
						case COMP_RIGIDBODY_2D:
						{
							Vec2f _velocity = { std::stof(tokens[1]), std::stof(tokens[2]) };
							Vec2f _acceleration = { std::stof(tokens[3]), std::stof(tokens[4]) };
							Float_32 _angular_velocity = std::stof(tokens[5]);
							Float_32 _angular_acceleration = std::stof(tokens[6]);
							Float_32 _mass_inverse = std::stof(tokens[7]);
							Bool_8 _is_kinematic = std::stoi(tokens[8]);
							Float_32 _drag = std::stof(tokens[9]);
							Float_32 _angular_drag = std::stof(tokens[10]);
							Float_32 _static_friction = std::stof(tokens[11]);
							Float_32 _dynamic_friction = std::stof(tokens[12]);
							Float_32 _coefficient_of_restitution = std::stof(tokens[13]);
							assert(transform_id);
							Uint_32 rigidbody_id = PhysicsModule::rigidbody2d_system.make(entity);
							if (rigidbody_id)
							{
								Rigidbody2D& rigidbody2d = PhysicsModule::rigidbody2d_system.get_component(rigidbody_id);
								rigidbody2d.velocity = _velocity;
								rigidbody2d.acceleration = _acceleration;
								rigidbody2d.angular_acceleration = _angular_acceleration;
								rigidbody2d.angular_velocity = _angular_velocity;
								rigidbody2d.transform_id = transform_id;
								rigidbody2d.mass_inverse = _mass_inverse;
								rigidbody2d.is_kinematic = _is_kinematic;
								rigidbody2d.drag = _drag;
								rigidbody2d.angular_drag = _angular_drag;
								rigidbody2d.static_friction = _static_friction;
								rigidbody2d.dynamic_friction = _dynamic_friction;
								rigidbody2d.coefficient_of_restitution = _coefficient_of_restitution;

							}
							else
								LOG(LOGTYPE_ERROR, "Max rigidbody2d count reached, couldn't load from scene graph");


						}
						break;
						case COMP_UNKNOWN:
							LOG(LOGTYPE_ERROR, "Couldn't determine componenet type : ", std::to_string(comp_type));
							break;
						}
					}
				}
			}
		}
	}


	std::vector<std::thread> loader_threads;

	void EntityGenerator::load_scenegraph(const std::string& scene_file_name) 
	{
		//auto s = sizeof(Material);


		std::cout << "Loading default material and texture" << std::endl;
		{
			auto performance_timer = PerformanceTimer();		Uint_32 mat = Material::load_material("Materials/Default.mat", true);
			assert(Material::material_creation_status); // default material has to be created for engine to work


		///////////////////

		//Uint_32 count = 500;
		//Float_32 x = -(count*1.f) / 2.f;
		//Float_32 y = -(count*1.25f) / 2.f;

		//for (; x < (count * 1.f)/2.f; x += 1.f)
		//{
		//	for (y = -(count*1.25f) / 2.f; y < (count * 1.25f)/2.f; y += 1.25f)
		//	{
		//		//make_animated_sprite({ x,y,0 }, "Animations/run.anim", "Materials/Tim.mat");
		//		make_sprite("Materials/Tim.mat", { x,y,0 }, RENDER_STATIC);
		//	}
		//}








		std::string scene_data = read_file( scene_file_name);
		std::stringstream input(scene_data);
		std::string entity_str;
		std::vector<Transform3D*> loaded_transforms;
		int no_of_graphics = 0;
		//std::vector<Uint_32> unbatched_grpahic_ids;
			while (std::getline(input, entity_str, '~')) // get an entity
			{

				//loader_threads.emplace_back(generate_entity_in_thraed, entity_str);
				trim(entity_str);
				std::stringstream ent(entity_str);
				std::string comp_str;
				std::string entity_name = "";
				std::getline(ent, entity_name);

				if (entity_name == "Materials")
				{
					
					while (std::getline(ent, comp_str))	// comp str is actually the name of the many materials in this case
					{
						std::string material_path = comp_str;
						trim(material_path);
						Material::load_material(material_path, false);
					}
				}
				else
				{
					//Entity* entity = EntityManagementSystem::entity_management_system->make_entity(entity_name);
					Uint_32 entity = entity_management_system->make_entity(entity_name);
					if (entity)
					{
						Uint_32 transform_id = 0;
						Uint_32 graphic_id = 0;
						Uint_32 animator_id = 0;
						while (std::getline(ent, comp_str)) // get a componenet in the entity
						{
							std::stringstream comp(comp_str);
							std::string token;
							std::vector<std::string> tokens;
							while (std::getline(comp, token, ','))
							{
								tokens.push_back(token);
							}
	
							if (tokens.size() > 0)
							{
								Int_32 comp_type = std::stoi(tokens[0], nullptr, 10);
								LOG(LOGTYPE_GENERAL, "COMP TYPE: ", std::to_string(comp_type));
								switch (comp_type)
								{
									/*case COMP_SPRITE:
									{
	
										Sprite* sprite = new Sprite(std::stoi(tokens[1]));
										sprite->add_to_renderer(renderer);
										entity->add_componenet(sprite);
										break;
									}*/
								case COMP_COLLIDER:
								{
									LOG(LOGTYPE_WARNING, "will make collider");
									// to be moved
									//Uint_32 collider_id = entity_management_system->make_collider_comp(entity);
									//Collider& col = colliders[collider_id];
	
									Uint_32 collider_id = PhysicsModule::collider_system.make(entity);
									Collider& col = PhysicsModule::collider_system.get_component(collider_id);
	
									col.graphic_id = graphic_id;
									col.transform_id = transform_id;
									col.type = (Shape2DTypes)std::atoi(tokens[1].c_str());
									for (int _i = 2; _i < tokens.size() - 1; _i += 2)
									{
										Vec2f p{ std::strtof(tokens[_i].c_str(), nullptr), std::strtof(tokens[_i + 1].c_str(), nullptr) };
										col.points[(_i / 2) - 1] = p;
										col.point_count++;
									}
	
									LOG(LOGTYPE_WARNING, "made collider");
	
								}
								break;
								case COMP_ANIMATOR:
								{
	
									animator_id = animator_system.make(entity);
									if (animator_id)
									{
										Animator& animator = animator_system.get_component(animator_id);
										animator.cur_anim_ind = std::stod(tokens[1]);
										animator.anim_transform_update_flags[0] = std::stod(tokens[2]);
										animator.anim_transform_update_flags[1] = std::stod(tokens[3]);
										animator.anim_transform_update_flags[2] = std::stod(tokens[4]);
	
										assert(transform_id);
										animator.id_transform = transform_id;
	
										for (int t_i = 5; t_i < tokens.size(); t_i++)
										{
											Uint_32 a_id = 0;
											trim(tokens[t_i]);
											if (a_id = Animator::load_animation(tokens[t_i]))
											{
												animator.add_animation(a_id);
												Animation& animation = animator.get_animation(a_id);
	
												for (Keyframe frame : animation.frames)
												{
													batched_texture_id_mutex.unlock();
													batched_texture_ids.push_back(frame.texture);
													batched_texture_id_mutex.unlock();
	
												}
											}
											else
												LOG(LOGTYPE_ERROR, "Couldn't create animator");
										}
									}
									/*
																animator_id = entity_management_system->make_animator_comp(entity);
																animators[animator_id].cur_anim_ind = std::stod(tokens[1]);
																animators[animator_id].anim_transform_update_flags[0] = std::stod(tokens[2]);
																animators[animator_id].anim_transform_update_flags[1] = std::stod(tokens[3]);
																animators[animator_id].anim_transform_update_flags[2] = std::stod(tokens[4]);*/
	
																//assert(transform_id);
																//	animators[animator_id].id_transform = transform_id;
																//	for (int t_i = 5; t_i < tokens.size(); t_i++)
																//	{
																//		Uint_32 a_id = 0;
																//		trim(tokens[t_i]);
																//		if (a_id = Animator::load_animation(tokens[t_i]))
																//		{
																//			animators[animator_id].add_animation(a_id);// Animator::animations_library.size() - 1;
																//			/*Uint_32 entity_2 = entity_management_system->make_entity();
																//			Uint_32 id_transform_2 = entity_management_system->make_transform_comp(entity_2);
																//			entity_management_system->set_parent_transform(id_transform, id_transform_2);
																//			id_transform = id_transform_2;*/
																//			
																//		}
																//		else
																//			LOG(LOGTYPE_ERROR, "Couldn't create animator");
																//	}
																	//assert(id_graphic);
																	//animators[id_animator].id_graphic = id_graphic;
																	/*Animator* animator = new Animator();
																	for (Int_32 i = 1; i < tokens.size(); i++)
																	{
																		animator->load_animation(tokens[i]);
																	}
																	entity->add_componenet(animator);*/
									break;
								}
								case COMP_CAMERA:	// since 2d engine, assume orthographic 
								{
									Float_32 left = std::stof(tokens[2]);
									Float_32 right = std::stof(tokens[3]);
									Float_32 bottom = std::stof(tokens[4]);
									Float_32 top = std::stof(tokens[5]);
									Float_32 _near = std::stof(tokens[6]);
									Float_32 _far = std::stof(tokens[7]);
									Float_32 _zoom = std::stof(tokens[8]);
									/*Uint_32 id_camera = entity_management_system->make_camera_comp(entity);
									cameras[id_camera].set_orthographic(left, right, bottom, top, _near, _far);
									assert(transform_id);
									cameras[id_camera].id_transform = transform_id;*/
									Uint_32 id_camera = camera_system.make(entity);
									Camera& camera = camera_system.get_component(id_camera);
									camera.set_orthographic(left, right, bottom, top, _near, _far);
									camera.zoom = _zoom;
									assert(transform_id);
									camera.id_transform = transform_id;
	
	
									/*if (std::stoi(tokens[1]) == ORTHOGRAPHIC)
									{
										Float_32 l = std::stof(tokens[2]);
										Float_32 r = std::stof(tokens[3]);
										Float_32 b = std::stof(tokens[4]);
										Float_32 t = std::stof(tokens[5]);
										Float_32 n = std::stof(tokens[6]);
										Float_32 f = std::stof(tokens[7]);
	
										Camera* camera = new Camera(l, r, b, t, n, f);
										entity->add_componenet(camera);
									}*/
									break;
								}
								case COMP_GRAPHICS:
								{
									Float_32 import_scale = (Float_32)std::atof(tokens[3].c_str());
									std::string material_path = tokens[1];
									trim(material_path);
									RenderTag render_tag = (RenderTag)std::atoi(tokens[2].c_str());
									assert(transform_id);
	
									//Comment for benchmarking purpose, Uncomment in build
	#ifdef EDITOR_MODE
									if (render_tag == RENDER_STATIC) render_tag = RENDER_UNTAGGED;
									//render_tag = RENDER_UNTAGGED;
	#endif
									if (animator_id)
										render_tag = RENDER_DYNAMIC;
	
									////////////////////////////
									//render_tag = RENDER_DYNAMIC;
	
									make_sprite(entity, import_scale, render_tag, animator_id, transform_id, material_path);
	
	
									break;
								}
								case COMP_LIGHT:
								{
									/*Uint_32 id_dir_light = entity_management_system->make_directional_light_comp(entity);
									directional_lights[id_dir_light].specular = 0.5f;
									directional_lights[id_dir_light].ambient = 0.3f;
									assert(transform_id);
									directional_lights[id_dir_light].id_transform = transform_id;*/
	
									/*DirectionalLight* light = new DirectionalLight(std::stof(tokens[1]), std::stof(tokens[2]));
									entity->add_componenet(light);*/
									break;
								}
								case COMP_TRANSFORM_3D:
								{
									transform_id = transform_system.make(entity);
									LOG(LOGTYPE_WARNING, std::to_string(transform_id));
									transform_id_mapping[std::stoi(tokens[12])] = transform_id;	//mapping for finding parents
									Transform3D& _transform = transform_system.get_component(transform_id);
									_transform.set_local_position(std::stof(tokens[1]), std::stof(tokens[2]), std::stof(tokens[3]));
									_transform.set_local_scale(std::stof(tokens[4]), std::stof(tokens[5]), std::stof(tokens[6]));
	
									Quaternion rot = { std::stof(tokens[7]), std::stof(tokens[8]), std::stof(tokens[9]), std::stof(tokens[10]) };
									_transform.set_local_rotation(rot);
									Uint32 parent_transform_id = transform_id_mapping[std::stoi(tokens[11])];
									if (parent_transform_id)
										entity_management_system->set_parent_transform(parent_transform_id, transform_id);
									//transforms[transform_id].update_transformation();
									break;
								}
								case COMP_RIGIDBODY_2D:
								{
									Vec2f _velocity = { std::stof(tokens[1]), std::stof(tokens[2]) };
									Vec2f _acceleration = { std::stof(tokens[3]), std::stof(tokens[4]) };
									Float_32 _angular_velocity = std::stof(tokens[5]);
									Float_32 _angular_acceleration = std::stof(tokens[6]);
									Float_32 _mass_inverse = std::stof(tokens[7]);
									Bool_8 _is_kinematic = std::stoi(tokens[8]);
									Float_32 _drag = std::stof(tokens[9]);
									Float_32 _angular_drag = std::stof(tokens[10]);
									Float_32 _static_friction = std::stof(tokens[11]);
									Float_32 _dynamic_friction = std::stof(tokens[12]);
									Float_32 _coefficient_of_restitution = std::stof(tokens[13]);
									assert(transform_id);
									Uint_32 rigidbody_id = PhysicsModule::rigidbody2d_system.make(entity);
									if (rigidbody_id)
									{
										Rigidbody2D& rigidbody2d = PhysicsModule::rigidbody2d_system.get_component(rigidbody_id);
										rigidbody2d.velocity = _velocity;
										rigidbody2d.acceleration = _acceleration;
										rigidbody2d.angular_acceleration = _angular_acceleration;
										rigidbody2d.angular_velocity = _angular_velocity;
										rigidbody2d.transform_id = transform_id;
										rigidbody2d.mass_inverse = _mass_inverse;
										rigidbody2d.is_kinematic = _is_kinematic;
										rigidbody2d.drag = _drag;
										rigidbody2d.angular_drag = _angular_drag;
										rigidbody2d.static_friction = _static_friction;
										rigidbody2d.dynamic_friction = _dynamic_friction;
										rigidbody2d.coefficient_of_restitution = _coefficient_of_restitution;
	
									}
									else
										LOG(LOGTYPE_ERROR, "Max rigidbody2d count reached, couldn't load from scene graph");
	
	
								}
								break;
								case COMP_UNKNOWN:
									LOG(LOGTYPE_ERROR, "Couldn't determine componenet type : ", std::to_string(comp_type));
									break;
								}
							}
						}
					}
				}
			}

			/*for (int i = 0; i < loader_threads.size(); i++)
				loader_threads[i].join();*/

			LOG(LOGTYPE_GENERAL, "will update transform");
			//entity_management_system->update_transforms();
			generate_batches();

			Texture::delete_all_texture_data();
		}
		std::cout << "hello world";
		//renderer->prepare_dynmic_batches(dynamic_batched_graphic_ids);
		// resolve transform hierarchy
		/*for (std::vector<Transform3D*>::iterator it = loaded_transforms.begin(); it != loaded_transforms.end(); it++)
		{
			
		}*/
	}

	void EntityGenerator::generate_batches()
	{
		LOG(LOGTYPE_GENERAL, "will make batches (static)");
		//renderer->prepare_batches(static_batched_graphic_ids, GL_STATIC_DRAW);
		renderer->prepare_array_textures(batched_texture_ids);
		//LOG(LOGTYPE_GENERAL, "will make batches (dynamic)");
		//renderer->prepare_batches(dynamic_batched_graphic_ids, GL_STREAM_DRAW);
	}
}