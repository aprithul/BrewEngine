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


	Entity* EntityGenerator::make_camera()
	{
		std::string entity_name = "Camera";
		Entity* camera_ent = EntityManagementSystem::entity_management_system->generate_entity(entity_name);
		Transform3D* camera_transform = new Transform3D();
		//Camera* camera = new Camera(16, 9, 0.1f, 100.f, 45.f, *camera_transform);
		Camera* camera = new Camera(-8, 8, -4.5f, 4.5f, -10, 10, *camera_transform);
		//camera->projection_type = ORTHOGRAPHIC;
		camera->transform.set_position(0.f, 1.f, -6.f);
		camera_ent->add_componenet(camera_transform);
		camera_ent->add_componenet(camera);
		return camera_ent;
	}

	Entity* EntityGenerator::make_animated_sprite_entity(const std::vector<std::string>& image_file_paths, int fps)
	{

		Transform3D* _transform = new Transform3D();
		auto rand_x = rand()%3 * (rand()%2==0?-1:1);
		auto rand_y = rand()%3 * (rand()%2==0?-1:1);
		_transform->set_position(rand_x, rand_y, 0);
		_transform->set_scale(1,1,1);

		Graphics* _graphics = renderer->generate_sprite_graphics(image_file_paths, std::string("sprite_mat")+"_"+image_file_paths[0]);

		Animator* _animator = new Animator();
		_animator->set_frame_rate(fps);

		Sprite* _sprite = new Sprite(0);
		_sprite->add_to_renderer(renderer);

		std::string entity_name = "Tim";
		Entity* _entity = EntityManagementSystem::entity_management_system->generate_entity(entity_name);
		_entity->add_componenet(_transform);
		_entity->add_componenet(_graphics);
		_entity->add_componenet(_sprite);
		_entity->add_componenet(_animator);

		//auto text = _entity->to_string();
		//write_to_file(text, "data", false);
		std::string grpah_file = "scene.graph";
		//load_scenegraph(grpah_file);
		//auto _graph_data = read_file("Scene.graph");
		//std::cout << _graph_data << std::endl;
		
		return _entity;
	}

	Entity* EntityGenerator::make_light_entity()
	{
		std::string entity_name = "Directional Light";
		Entity* light_ent = EntityManagementSystem::entity_management_system->generate_entity(entity_name);
        Transform3D* light_tr = new Transform3D();
        light_tr->set_rotation(Vector3<float>(0,0,90));
        DirectionalLight* light = new DirectionalLight(0.5f, 0.3f);
        light_ent->add_componenet(light);
        light_ent->add_componenet(light_tr);
		return light_ent;
	}


	void EntityGenerator::load_scenegraph(std::string& scene_file_name) 
	{
		std::string scene_data = read_file(scene_file_name);
		std::stringstream input(scene_data);
		std::string entity_str;
		
		while (std::getline(input, entity_str, '~')) // get an entity
		{
			std::stringstream ent(entity_str);
			std::string comp_str;
			Entity* entity = new Entity();

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
					int comp_type = std::stoi(tokens[0], nullptr, 10);
					switch (comp_type)
					{
						case COMP_SPRITE:

							//std::vector<std::string> texture_paths(tokens.begin() + 1, tokens.end());
							//Sprite* sprite = renderer->generate_sprite_graphics(texture_paths,)
							break;
						case COMP_ANIMATOR:
							break;
						case COMP_CAMERA:
							break;
						case COMP_GRAPHICS:	
							
							break;
						case COMP_LIGHT:
							break;
						case COMP_TRANSFORM_3D:
						{
							Transform3D* _transform = new Transform3D();
							_transform->set_position(std::stof(tokens[1]), std::stof(tokens[2]), std::stof(tokens[3]));
							_transform->set_scale(std::stof(tokens[4]), std::stof(tokens[5]), std::stof(tokens[6]));
							_transform->set_rotation(std::stof(tokens[7]), std::stof(tokens[8]), std::stof(tokens[9]));
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
	}
}