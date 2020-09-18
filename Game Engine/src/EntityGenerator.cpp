/*
 * EntityGenerator.cpp
 *
 *  Created on: Sep 15, 2020
 *      Author: cobre
 */

#include "EntityGenerator.hpp"

namespace PrEngine{

	EntityGenerator::EntityGenerator(RendererOpenGL2D* renderer)
	{
		this->renderer = renderer;

	}

	Entity* EntityGenerator::make_animated_sprite_entity(const std::vector<std::string>& image_file_paths, int fps)
	{
		Transform3D* _transform = new Transform3D();
		auto rand_x = rand()%3 * (rand()%2==0?-1:1);
		auto rand_y = rand()%3 * (rand()%2==0?-1:1);
		_transform->set_position(rand_x, rand_y, 0);
		_transform->set_scale(1,1,1);

		Sprite* _sprite = renderer->generate_graphics_sprite(get_resource_path(image_file_paths[0]), std::string("sprite_mat")+"_"+image_file_paths[0]);
		for(int i=0; i<image_file_paths.size(); i++)
		{
			Texture* _t = Texture::load_texture(image_file_paths[i]);
			if(_t != nullptr)
				_sprite->animator.add_frame(_t);
		}
		_sprite->animator.set_frame_rate(fps);


		Animator* _animator = &(_sprite->animator);
		Graphics* _graphics = &(_sprite->graphics);
		_graphics->model = &(_transform->get_transformation());
		_graphics->normal = &(_transform->get_rotation_transformation());

		Entity* _entity = new Entity();
		_entity->add_componenet(_transform);
		_entity->add_componenet(_graphics);
		_entity->add_componenet(_sprite);
		_entity->add_componenet(_animator);
		Entity* _ent =  EntityManagementSystem::entity_management_system->assign_id_and_store_entity(*_entity);
		if(_ent != nullptr)
		{
			// serialize
			FILE* fp;
			std::string text = std::to_string(EntityTypes::ANIMATED_SPRITE)+",";
			for(int i=0; i<image_file_paths.size(); i++)
			{
				text += (image_file_paths[i]+",");
			}
			text+=std::to_string(fps)+",";
			text += _transform->serialize()+"\n";

			fp = write_to_file(text.c_str(), "Scene.graph", fp);
			close_file(fp);

		}
		return _ent;
	}

	Entity* EntityGenerator::make_light_entity()
	{
        Entity* light_ent = new Entity();
        Transform3D* light_tr = new Transform3D();
        light_tr->set_rotation(Vector3<float>(0,0,90));
        DirectionalLight* light = new DirectionalLight(0.5f, 0.3f);
        light_ent->add_componenet(light);
        light_ent->add_componenet(light_tr);
        return EntityManagementSystem::entity_management_system->assign_id_and_store_entity(*light_ent);
	}
}
