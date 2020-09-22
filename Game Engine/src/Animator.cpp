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
		this->on_frame_change_timer = nullptr;
		this->frame_rate = 1;
		this->frame_delta = 999999;	
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

		}
		else
		{
			LOG(LOGTYPE_ERROR, "Entity with id ", std::to_string(entity_id), " couldn't be found");
		}
	}

	void Animator::set_frame_rate(int frame_rate)
	{
		this->frame_rate = frame_rate;
		this->frame_delta =  (1.0/frame_rate);
		LOG(LOGTYPE_GENERAL,"setting animation frame delta to ", std::to_string(frame_delta));
		if(on_frame_change_timer != nullptr)
		{
			this->on_frame_change_timer->target_duration = frame_delta;
		}
		else{
			this->on_frame_change_timer = Time::make_timer(frame_delta, std::bind(&Animator::on_next_frame, this), true);

		}
	}

	void Animator::update()
	{

	}

	void Animator::on_next_frame()
	{
		//current_frame_index = (current_frame_index +1)%animation_frames->size();
		//material->diffuse_texture = (*animation_frames)[current_frame_index];
	}

	std::string Animator::to_string()
	{

		std::string text = std::to_string(COMP_ANIMATOR)+","+std::to_string(COMP_ANIMATOR)+","+std::to_string(this->frame_rate);
		return text;
	}


}


