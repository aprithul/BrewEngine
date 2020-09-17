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

	Entity* EntityGenerator::make_sprite_entity(const std::string& image_file_path)
	{
		Transform3D* _transform = new Transform3D();
		auto rand_x = rand()%3 * (rand()%2==0?-1:1);
		auto rand_y = rand()%3 * (rand()%2==0?-1:1);
		_transform->set_position(rand_x, rand_y, 0);
		_transform->set_scale(1,1,1);
		Graphics* _graphics= renderer->generate_graphics_sprite(get_resource_path(image_file_path), std::string("sprite_mat")+"_"+image_file_path);
		_graphics->model = &(_transform->get_transformation());
		_graphics->normal = &(_transform->get_rotation_transformation());
		Entity* _entity = new Entity();
		_entity->add_componenet(_transform);
		_entity->add_componenet(_graphics);
		return EntityManagementSystem::entity_management_system->assign_id_and_store_entity(*_entity);
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
