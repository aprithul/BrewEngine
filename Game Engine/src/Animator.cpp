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
		this->animator_time = 0;
		this->transform = nullptr;
		this->current_animation = nullptr;

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
		animator_time += Time::Frame_time;
		Keyframe frame = current_animation->frames[current_frame_index];
		if (frame.timestamp <= animator_time * animation_speed)
		{
			transform->set_position(frame.position);
			transform->set_scale(frame.scale);
			transform->set_rotation(frame.rotation);
			material->diffuse_texture = frame.texture;
			current_frame_index = (current_frame_index+1)%((int)(current_animation->frames.size()));
			
			// if we've looped around restart timer
			if (current_frame_index == 0)
			{
				animator_time = 0;
			}
		}
	}

	void Animator::load_animation(std::string& file_name)
	{
		this->animations.emplace(file_name,file_name);
		if(current_animation == nullptr)
			current_animation = &animations.begin()->second;
	}

	std::string Animator::to_string()
	{
		std::string text = std::to_string(COMP_ANIMATOR);
		for (std::unordered_map<std::string, Animation>::iterator it = animations.begin(); it != animations.end(); it++)
		{
			text += "," + it->second.clip_name;
		}
		return text;
	}




}


