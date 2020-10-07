/*
 * EntityGenerator.cpp
 *
 *  Created on: Sep 15, 2020
 *      Author: cobre
 */

#include "EntityGenerator.hpp"

namespace PrEngine{

	EntityGenerator::EntityGenerator()
	{
		this->renderer = (RendererOpenGL2D*)Engine::engine->get_module("Renderer");
	}

	Uint_32 EntityGenerator::make_camera()
	{
		//std::string entity_name = "Camera";
		Uint_32 entity = entity_management_system->make_entity();
		
		Uint_32 id_transform = entity_management_system->make_transform_comp(entity);
		transforms[id_transform].position = Vector3<Float_32>( 0.f, 1.f, -6.f);
		
		Uint_32 id_camera = entity_management_system->make_camera_comp(entity);
		cameras[id_camera].set_orthographic(-8, 8, -4.5f, 4.5f, -10, 10);
		cameras[id_camera].id_transform = id_transform;

		//auto text = camera_ent->to_string();
		//write_to_file(text, "scene.graph", false, true);
		return entity;
	}

	Uint_32 EntityGenerator::make_animated_sprite_entity(const std::string& image_file_path)
	{
		Uint_32 entity = entity_management_system->make_entity();

		auto rand_x = rand()%3 * (rand()%2==0?-1:1);
		auto rand_y = rand()%3 * (rand()%2==0?-1:1);
		
		Uint_32 id_transform = entity_management_system->make_transform_comp(entity);
		transforms[id_transform].position = Vector3<Float_32>(rand_x, rand_y, -6.f);
		transforms[id_transform].scale = Vector3<Float_32>(1,1,1);

		Uint_32 id_graphic = entity_management_system->make_graphic_comp(entity);
		renderer->generate_sprite_graphics(id_graphic, image_file_path, std::string("sprite_mat_")+image_file_path);
		graphics[id_graphic].id_transform = id_transform;

		Animator::load_animation("Animations" + PATH_SEP + "my.anim");
		Uint_32 id_animator = entity_management_system->make_animator_comp(entity);
		Animation anim = Animator::animations.begin()->second;
		animators[id_animator].current_animation = anim;
		animators[id_animator].id_transform = id_transform;
		animators[id_animator].id_graphic = id_graphic;

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
        transforms[id_transform].rotation = Vector3<Float_32>(0,0,90);

		Uint_32 id_dir_light = entity_management_system->make_directional_light_comp(entity);
		directional_lights[id_dir_light].specular = 0.5f;
		directional_lights[id_dir_light].ambient = 0.3f;
		directional_lights[id_dir_light].id_transform = id_transform;

		//auto text = light_ent->to_string();
		//write_to_file(text, "scene.graph", false, true);

		return entity;
	}

	/*
	void EntityGenerator::load_scenegraph(std::string& scene_file_name) 
	{
		std::string scene_data = read_file(scene_file_name);
		std::stringstream input(scene_data);
		std::string entity_str;
		std::vector<Transform3D*> loaded_transforms;


		while (std::getline(input, entity_str, '~')) // get an entity
		{
			std::stringstream ent(entity_str);
			std::string comp_str;
			std::string entity_name = "-.-";
			Entity* entity = EntityManagementSystem::entity_management_system->make_entity(entity_name);

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
						case COMP_SPRITE:
						{	
							Sprite* sprite = new Sprite(std::stoi(tokens[1]));
							sprite->add_to_renderer(renderer);
							entity->add_componenet(sprite);
							break;
						}
						case COMP_ANIMATOR:
						{	
							Animator* animator = new Animator();
							for (Int_32 i = 1; i < tokens.size(); i++)
							{
								animator->load_animation(tokens[i]);
							}
							entity->add_componenet(animator);
							break;
						}
						case COMP_CAMERA:	// since 2d engine, assume orthographic 
						{
							if (std::stoi(tokens[1]) == ORTHOGRAPHIC)
							{
								Float_32 l = std::stof(tokens[2]);
								Float_32 r = std::stof(tokens[3]);
								Float_32 b = std::stof(tokens[4]);
								Float_32 t = std::stof(tokens[5]);
								Float_32 n = std::stof(tokens[6]);
								Float_32 f = std::stof(tokens[7]);

								Camera* camera = new Camera(l, r, b, t, n, f);
								entity->add_componenet(camera);
							}
							break;
						}
						case COMP_GRAPHICS:	
						{
							Graphic* graphics = renderer->generate_sprite_graphics(tokens[1], "sprite_mat_" + tokens[1]);
							entity->add_componenet(graphics);
							break;
						}
						case COMP_LIGHT:
						{
							DirectionalLight* light = new DirectionalLight(std::stof(tokens[1]), std::stof(tokens[2]));
							entity->add_componenet(light);
							break;
						}	
						case COMP_TRANSFORM_3D:
						{
							Transform3D* _transform = new Transform3D();
							_transform->set_position(std::stof(tokens[1]), std::stof(tokens[2]), std::stof(tokens[3]));
							_transform->set_scale(std::stof(tokens[4]), std::stof(tokens[5]), std::stof(tokens[6]));
							_transform->set_rotation(std::stof(tokens[7]), std::stof(tokens[8]), std::stof(tokens[9]));
							
							Int_32 id = std::stoi(tokens[10]);
							if (id != -1)	// when loaded from scene graph
							{
								_transform->id = id;
							}
							else	//when created in scene
							{
								_transform->id = entity->id;
							}

							loaded_transforms.push_back(_transform);
							entity->add_componenet(_transform);
							break;
						}
						case COMP_UNKNOWN:
							LOG(LOGTYPE_ERROR, "Couldn't determine componenet type : ", std::to_string(comp_type));
							break;
					}
				}
			}
		}

		// resolve transform hierarchy
		for (std::vector<Transform3D*>::iterator it = loaded_transforms.begin(); it != loaded_transforms.end(); it++)
		{
			
		}
	}
	*/
}