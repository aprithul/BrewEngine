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
		get_transform(id_transform).position = Vector3<Float_32>( 0.f, 1.f, -6.f);
		
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

		auto rand_x = rand()%3 * (rand()%2==0?-1:1);
		auto rand_y = rand()%3 * (rand()%2==0?-1:1);
		
		Uint_32 id_transform = entity_management_system->make_transform_comp(entity);
		get_transform(id_transform).position = Vector3<Float_32>(rand_x, rand_y, -6.f);
		get_transform(id_transform).scale = Vector3<Float_32>(1,1,1);

		Uint_32 id_graphic = entity_management_system->make_graphic_comp(entity);
		//renderer->generate_sprite_graphics(id_graphic, image_file_path, std::string("sprite_mat_")+image_file_path);
		Uint_32 mat_id = Material::load_material(material_name, true);
		graphics[id_graphic].element.material = mat_id;

		renderer->generate_sprite_graphics(id_graphic);

		graphics[id_graphic].id_transform = id_transform;

		Animator::load_animation("Animations" + PATH_SEP + "my.anim");
		Uint_32 id_animator = entity_management_system->make_animator_comp(entity);
		Animation anim = Animator::animations_library.begin()->second;
		animators[id_animator].animation = anim;
		animators[id_animator].id_transform = id_transform;
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
		get_transform(id_transform).rotation = Vector3<Float_32>(0,0,90);

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

		graphics[g_id].id_transform = t_id;

		return e;
	}
	void EntityGenerator::load_scenegraph(const std::string& scene_file_name) 
	{
		std::string scene_data = read_file(scene_file_name);
		std::stringstream input(scene_data);
		std::string entity_str;
		std::vector<Transform3D*> loaded_transforms;
		std::vector<Uint_32> static_batched_graphic_ids;
		std::vector<Uint_32> dynamic_batched_graphic_ids;

		std::vector<Uint_32> unbatched_grpahic_ids;
		while (std::getline(input, entity_str, '~')) // get an entity
		{
			std::stringstream ent(entity_str);
			std::string comp_str;
			std::string entity_name = "-.-";
			//Entity* entity = EntityManagementSystem::entity_management_system->make_entity(entity_name);
			Uint_32 entity = entity_management_system->make_entity();
			Uint_32 id_transform = 0;
			Uint_32 id_graphic = 0;
			Uint_32 id_animator = 0;
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
						case COMP_ANIMATOR:
						{	
							if (Animator::load_animation(tokens[1]))
							{
								id_animator = entity_management_system->make_animator_comp(entity);
								animators[id_animator].animation = Animator::animations_library[tokens[1]];

								assert(id_transform);
				
								/*Uint_32 entity_2 = entity_management_system->make_entity();
								Uint_32 id_transform_2 = entity_management_system->make_transform_comp(entity_2);
								entity_management_system->set_parent_transform(id_transform, id_transform_2);
								id_transform = id_transform_2;*/
								animators[id_animator].id_transform = id_transform;
							}
							else
								LOG(LOGTYPE_ERROR, "Couldn't create animator");
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
							assert(id_transform);
							cameras[id_camera].id_transform = id_transform;

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
							id_graphic = entity_management_system->make_graphic_comp(entity);
							
							RenderTag render_tag = (RenderTag)std::atoi(tokens[2].c_str());
#ifdef EDITOR_MODE
							//if (render_tag == RENDER_STATIC) render_tag = RENDER_DYNAMIC;
							render_tag = RENDER_UNTAGGED;
#endif
							if (id_animator)
								render_tag = RENDER_DYNAMIC;

							graphics[id_graphic].tag = render_tag;
							std::string material_name = tokens[1];
							Bool_8 create_gl_texture = render_tag == RENDER_UNTAGGED;
							if (create_gl_texture)
								LOG(LOGTYPE_GENERAL,"");
							Uint_32 mat_id = Material::load_material(material_name, create_gl_texture);
							graphics[id_graphic].element.material = mat_id;

						assert(id_transform);
							graphics[id_graphic].id_transform = id_transform;
							graphics[id_graphic].id_animator = id_animator;

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
										renderer->generate_sprite_graphics(id_graphic);


								}
									break;
								case RENDER_STATIC:
									static_batched_graphic_ids.push_back(id_graphic);
									//renderer->generate_batched_sprite_graphics(id_graphic);
									break;
								case RENDER_DYNAMIC:
									//renderer->generate_sprite_graphics(id_graphic);
									dynamic_batched_graphic_ids.push_back(id_graphic);
									break;
							}



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
							assert(id_transform);
							directional_lights[id_dir_light].id_transform = id_transform;

							/*DirectionalLight* light = new DirectionalLight(std::stof(tokens[1]), std::stof(tokens[2]));
							entity->add_componenet(light);*/
							break;
						}	
						case COMP_TRANSFORM_3D:
						{
							id_transform = entity_management_system->make_transform_comp(entity);
							LOG(LOGTYPE_WARNING, std::to_string(id_transform));
							transform_id_mapping[std::stoi(tokens[11])] = id_transform;	//mapping for finding parents
							get_transform(id_transform).position = Vector3<Float_32>(std::stof(tokens[1]), std::stof(tokens[2]), std::stof(tokens[3]));
							get_transform(id_transform).scale = Vector3<Float_32>(std::stof(tokens[4]), std::stof(tokens[5]), std::stof(tokens[6]));
							get_transform(id_transform).rotation = Vector3<Float_32>(std::stof(tokens[7]), std::stof(tokens[8]), std::stof(tokens[9]));
							Uint32 parent_transform_id =  transform_id_mapping[std::stoi(tokens[10])];	
							if(parent_transform_id)
								entity_management_system->set_parent_transform(parent_transform_id, id_transform);
							get_transform(id_transform).update_transformation();
							break;
						}
						case COMP_UNKNOWN:
							LOG(LOGTYPE_ERROR, "Couldn't determine componenet type : ", std::to_string(comp_type));
							break;
					}
				}
			}
		}

		entity_management_system->update_transforms();
		renderer->prepare_batches(static_batched_graphic_ids, GL_STATIC_DRAW);
		renderer->prepare_batches(dynamic_batched_graphic_ids, GL_DYNAMIC_DRAW);
		//renderer->prepare_dynmic_batches(dynamic_batched_graphic_ids);
		// resolve transform hierarchy
		/*for (std::vector<Transform3D*>::iterator it = loaded_transforms.begin(); it != loaded_transforms.end(); it++)
		{
			
		}*/
	}
	
}