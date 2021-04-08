/*
 * EntityGenerator.cpp
 *
 *  Created on: Sep 15, 2020
 *      Author: Aniruddha Prithul
 */

#include "EntityGenerator.hpp"

namespace PrEngine{

	std::unordered_map<int, int> EntityGenerator::transform_id_mapping;

	EntityGenerator::EntityGenerator()
	{
		transform_id_mapping[0] = 0;
	}

	Uint_32 EntityGenerator::make_camera_orthographic(Uint_32 width, Uint_32 height)
	{
		//std::string entity_name = "Camera";
		Uint_32 entity = entity_management_system->make_entity();
		
		Uint_32 id_transform = entity_management_system->make_transform_comp(entity);
		get_transform(id_transform).position = Point3d{ 0.f, 1.f, -6.f };
		
		Uint_32 id_camera = entity_management_system->make_camera_comp(entity);
		cameras[id_camera].set_orthographic(-(width / 2.f), (width / 2.f), -(height / 2.f), (height / 2.f), -10, 10);
		cameras[id_camera].id_transform = id_transform;

		//auto text = camera_ent->to_string();
		//write_to_file(text, "scene.graph", false, true);
		return entity;
	}

	Uint_32 EntityGenerator::make_animated_sprite_entity(const std::string& material_name)
	{
		Uint_32 entity = entity_management_system->make_entity();

		Float_32 rand_x = rand()%3 * (rand()%2==0?-1:1);
		Float_32 rand_y = rand()%3 * (rand()%2==0?-1:1);
		
		Uint_32 id_transform = entity_management_system->make_transform_comp(entity);
		get_transform(id_transform).position = Point3d{ rand_x, rand_y, -6.f };
		get_transform(id_transform).scale = Vec3f(1,1,1);

		Uint_32 id_graphic = entity_management_system->make_graphic_comp(entity);
		//renderer->generate_sprite_graphics(id_graphic, image_file_path, std::string("sprite_mat_")+image_file_path);
		Uint_32 mat_id = Material::load_material(material_name, true);
		graphics[id_graphic].element.material = mat_id;

		renderer->generate_sprite_graphics(id_graphic);

		graphics[id_graphic].transform_id = id_transform;

		std::string _anim_name = "Animations" + PATH_SEP + "my.anim";
		Animator::load_animation(_anim_name);
		Uint_32 id_animator = entity_management_system->make_animator_comp(entity);
		//Animation anim = Animator::animations_library.begin()->second;
		//animators[id_animator].animation = anim;
		//animators[id_animator].id_transform = id_transform;
		//animators[id_animator].id_graphic = id_graphic;

		//sprites[id].add_to_renderer(renderer);

		//std::string entity_name = "Tim";
		
		//_entity->add_componenet(_transform);
		//_entity->add_componenet(_graphics);
		//_entity->add_componenet(_sprite);
		//_entity->add_componenet(_animator);

		//auto text = _entity->to_string();
		//write_to_file(text, "scene.graph", false, true);
		//std::string grpah_file = "scene.graph";
		//write_to_file(text, "scene.graph", false);
		//load_scenegraph(grpah_file);
		//auto _graph_data = read_file("Scene.graph");
		//std::cout << _graph_data << std::endl;
		
		return entity;
		//std::string graph_file = "scene.graph";
		//load_scenegraph(graph_file);
	}

	Uint_32 EntityGenerator::make_light_entity()
	{
		Uint_32 entity = entity_management_system->make_entity();

		Uint_32 id_transform = entity_management_system->make_transform_comp(entity);
		get_transform(id_transform).rotation = Vec3f(0,0,90);

		Uint_32 id_dir_light = entity_management_system->make_directional_light_comp(entity);
		directional_lights[id_dir_light].specular = 0.5f;
		directional_lights[id_dir_light].ambient = 0.3f;
		directional_lights[id_dir_light].id_transform = id_transform;

		//auto text = light_ent->to_string();
		//write_to_file(text, "scene.graph", false, true);

		return entity;
	}

	Uint_32 EntityGenerator::make_graphics_entity(const std::string& material_name)
	{
		auto e = entity_management_system->make_entity();
		auto t_id = entity_management_system->make_transform_comp(e);

		auto g_id = entity_management_system->make_graphic_comp(e);
		//renderer->generate_sprite_graphics(g_id, image_file_path, std::string("sprite_mat_") + image_file_path);

		Uint_32 mat_id = Material::load_material(material_name, true);
		graphics[g_id].element.material = mat_id;

		renderer->generate_sprite_graphics(g_id);

		graphics[g_id].transform_id = t_id;

		return e;
	}

#ifdef  EDITOR_MODE


	void load_all_resrources()
	{

	}
#endif //  EDITOR_MODE

	void EntityGenerator::load_scenegraph(const std::string& scene_file_name) 
	{
		auto s = sizeof(Material);

		std::string scene_data = read_file(scene_file_name);
		std::stringstream input(scene_data);
		std::string entity_str;
		std::vector<Transform3D*> loaded_transforms;
		std::vector<Uint_32> static_batched_graphic_ids;
		std::vector<Uint_32> dynamic_batched_graphic_ids;

		std::vector<Uint_32> unbatched_grpahic_ids;
		while (std::getline(input, entity_str, '~')) // get an entity
		{
			trim(entity_str);
			std::stringstream ent(entity_str);
			std::string comp_str;
			std::string entity_name = "-.-";
			//Entity* entity = EntityManagementSystem::entity_management_system->make_entity(entity_name);
			Uint_32 entity = entity_management_system->make_entity();
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
					LOG(LOGTYPE_GENERAL, "COMP TYPE: ",std::to_string(comp_type));
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
							Uint_32 collider_id = entity_management_system->make_collider_comp(entity);
							Collider& col = colliders[collider_id];
							col.graphic_id = graphic_id;
							col.transform_id = transform_id;
							col.collision_shape.type = (Shape2DTypes)std::atoi(tokens[1].c_str());
							for (int _i = 2; _i < tokens.size(); _i+=2)
							{
								Vec2f p{ std::strtof(tokens[_i].c_str(), nullptr), std::strtof(tokens[_i + 1].c_str(), nullptr) };
								col.collision_shape.points[(_i/2) - 1] = p;
								col.collision_shape.point_count++;
							}
							LOG(LOGTYPE_WARNING, "made collider");

						}
						break;
						case COMP_ANIMATOR:
						{	
							animator_id = entity_management_system->make_animator_comp(entity);
							animators[animator_id].cur_anim_ind = std::stod(tokens[1]);
							animators[animator_id].anim_flags[0] = std::stod(tokens[2]);
							animators[animator_id].anim_flags[1] = std::stod(tokens[3]);
							animators[animator_id].anim_flags[2] = std::stod(tokens[4]);

						assert(transform_id);
							animators[animator_id].id_transform = transform_id;
							for (int t_i = 5; t_i < tokens.size(); t_i++)
							{
								Uint_32 a_id = 0;
								if (a_id = Animator::load_animation(tokens[t_i]))
								{
									animators[animator_id].add_animation(a_id);// Animator::animations_library.size() - 1;
									/*Uint_32 entity_2 = entity_management_system->make_entity();
									Uint_32 id_transform_2 = entity_management_system->make_transform_comp(entity_2);
									entity_management_system->set_parent_transform(id_transform, id_transform_2);
									id_transform = id_transform_2;*/
									
								}
								else
									LOG(LOGTYPE_ERROR, "Couldn't create animator");
							}
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
							Float_32 l = std::stof(tokens[2]);
							Float_32 r = std::stof(tokens[3]);
							Float_32 b = std::stof(tokens[4]);
							Float_32 t = std::stof(tokens[5]);
							Float_32 n = std::stof(tokens[6]);
							Float_32 f = std::stof(tokens[7]);

							Uint_32 id_camera = entity_management_system->make_camera_comp(entity);
							cameras[id_camera].set_orthographic(l, r, b, t, n, f);
							assert(transform_id);
							cameras[id_camera].id_transform = transform_id;

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
							graphic_id = entity_management_system->make_graphic_comp(entity);

							RenderTag render_tag = (RenderTag)std::atoi(tokens[2].c_str());
							RenderTag future_render_tag = render_tag;
							Graphic::editor_data[graphic_id] = { (Float_32)std::atof(tokens[3].c_str()), future_render_tag };
#ifdef EDITOR_MODE	
							//if (render_tag == RENDER_STATIC) render_tag = RENDER_DYNAMIC;
							render_tag = RENDER_UNTAGGED;
#endif
							if (animator_id)
								render_tag = RENDER_DYNAMIC;

							graphics[graphic_id].tag = render_tag;
							//graphics[id_graphic].future_tag = future_render_tag;
							std::string material_name = tokens[1];
							trim(material_name);
							Bool_8 create_gl_texture = render_tag == RENDER_UNTAGGED;
							if (create_gl_texture)
								LOG(LOGTYPE_GENERAL,"");
							Uint_32 mat_id = Material::load_material(material_name, create_gl_texture);
							LOG(LOGTYPE_GENERAL, "mat created");

							graphics[graphic_id].element.material = mat_id;

						assert(transform_id);
							graphics[graphic_id].transform_id = transform_id;
							graphics[graphic_id].animator_id = animator_id;

							
							LOG(LOGTYPE_GENERAL, "will add to batch");


							switch (render_tag)
							{
								case RENDER_UNTAGGED:	
								{	//renderer->generate_sprite_graphics(id_graphic);

									// we always batch animationn frames
									//if (id_animator)
									//{
									//	//entity_management_system->update_transforms();
									//	//animators[id_animator].animation.frames;
									//	std::vector<Uint_32> anim_graphic;
									//	anim_graphic.push_back(id_graphic);
									//	renderer->prepare_batches(anim_graphic, GL_DYNAMIC_DRAW);
									//	batched_graphics.back().id_transform = id_transform;
									//	batched_graphics.back().id_animator = id_animator;
									//}
									//else
									LOG(LOGTYPE_GENERAL, "creating sprite graphics");

										renderer->generate_sprite_graphics(graphic_id);


								}
									break;
								case RENDER_STATIC:
									static_batched_graphic_ids.push_back(graphic_id);
									//renderer->generate_batched_sprite_graphics(id_graphic);
									break;
								case RENDER_DYNAMIC:
									//renderer->generate_sprite_graphics(id_graphic);
									dynamic_batched_graphic_ids.push_back(graphic_id);
									break;
							}


							LOG(LOGTYPE_GENERAL, "added to batch list");

							//braid\\tim_run\\0.gif

							/*Graphic* graphics = renderer->generate_sprite_graphics(tokens[1], "sprite_mat_" + tokens[1]);
							entity->add_componenet(graphics);*/
							break;
						}
						case COMP_LIGHT:
						{
							Uint_32 id_dir_light = entity_management_system->make_directional_light_comp(entity);
							directional_lights[id_dir_light].specular = 0.5f;
							directional_lights[id_dir_light].ambient = 0.3f;
							assert(transform_id);
							directional_lights[id_dir_light].id_transform = transform_id;

							/*DirectionalLight* light = new DirectionalLight(std::stof(tokens[1]), std::stof(tokens[2]));
							entity->add_componenet(light);*/
							break;
						}	
						case COMP_TRANSFORM_3D:
						{
							transform_id = entity_management_system->make_transform_comp(entity);
							LOG(LOGTYPE_WARNING, std::to_string(transform_id));
							transform_id_mapping[std::stoi(tokens[11])] = transform_id;	//mapping for finding parents
							transforms[transform_id].position = Point3d{ std::stof(tokens[1]), std::stof(tokens[2]), std::stof(tokens[3]) };
							transforms[transform_id].scale = Vec3f(std::stof(tokens[4]), std::stof(tokens[5]), std::stof(tokens[6]));
							transforms[transform_id].rotation = Vec3f(std::stof(tokens[7]), std::stof(tokens[8]), std::stof(tokens[9]));
							Uint32 parent_transform_id =  transform_id_mapping[std::stoi(tokens[10])];	
							if(parent_transform_id)
								entity_management_system->set_parent_transform(parent_transform_id, transform_id);
							transforms[transform_id].update_transformation();
							break;
						}
						case COMP_UNKNOWN:
							LOG(LOGTYPE_ERROR, "Couldn't determine componenet type : ", std::to_string(comp_type));
							break;
					}
				}
			}
		}

		LOG(LOGTYPE_GENERAL, "will update transform");
		entity_management_system->update_transforms();
		LOG(LOGTYPE_GENERAL, "will make batches (static");
		renderer->prepare_batches(static_batched_graphic_ids, GL_STATIC_DRAW);
		LOG(LOGTYPE_GENERAL, "will make batches (dynamic)");
		renderer->prepare_batches(dynamic_batched_graphic_ids, GL_STREAM_DRAW);

		Texture::delete_all_texture_data();
		//renderer->prepare_dynmic_batches(dynamic_batched_graphic_ids);
		// resolve transform hierarchy
		/*for (std::vector<Transform3D*>::iterator it = loaded_transforms.begin(); it != loaded_transforms.end(); it++)
		{
			
		}*/
	}
	
}