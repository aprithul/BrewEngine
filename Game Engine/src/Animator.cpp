/*
 * Animator.cpp
 *
 *  Created on: Sep 17, 2020
 *      Author: cobre
 */

#include "Animator.hpp"
#include "EntityManagementSystemModule.hpp"
#include "Graphics.hpp"

namespace PrEngine
{
	Animator::Animator():Component(COMP_ANIMATOR)
	{
		this->material = nullptr;
		this->animation_speed = 1.f;
		this->current_frame_index = 0;
	}

	Animator::~Animator()
	{

	}

	void Animator::start()
	{
		auto _entity = EntityManagementSystem::entity_management_system->get_entity(this->entity_id);
		if (_entity != nullptr)
		{
			if (_entity->has_component[COMP_GRAPHICS])
			{
				auto graphics = (Graphics*)_entity->components[COMP_GRAPHICS];
				this->material = graphics->elements.back().material;
			}
			else
			{
				LOG(LOGTYPE_ERROR, "couldn't get graphics componenet in animator");

			}

			if (_entity->has_component[COMP_TRANSFORM_3D])
			{
				this->transform = (Transform3D*)_entity->components[COMP_TRANSFORM_3D];
			}
			else
			{
				LOG(LOGTYPE_ERROR, "couldn't get transform componenet in animator");

			}

		}
		else
		{
			LOG(LOGTYPE_ERROR, "Entity with id ", std::to_string(entity_id), " couldn't be found");
		}
	}

	
	void Animator::update()
	{

	
	}

	void Animator::load_animation(std::string& file_name)
	{
		this->aniamtions.emplace(file_name,file_name);
	}

	std::string Animator::to_string()
	{

		std::string text = std::to_string(COMP_ANIMATOR) + ",";
		return text;
	}




}


