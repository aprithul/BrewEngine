/*
 * Animator.cpp
 *
 *  Created on: Sep 17, 2020
 *      Author: cobre
 */

#include "Animator.hpp"

namespace PrEngine
{
	Animator::Animator(Material* material):Component(COMP_ANIMATOR)
	{
		this->material = material;
		this->on_frame_change_timer = nullptr;
	}

	Animator::~Animator()
	{

	}

	void Animator::add_frame(Texture* frame)
	{
		animation_frames.push_back(frame);
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
		current_frame_index = (current_frame_index +1)%animation_frames.size();
		material->diffuse_texture = animation_frames[current_frame_index];
	}

	std::string Animator::to_string()
	{

		std::string text = std::to_string(COMP_ANIMATOR)+","+std::to_string(COMP_ANIMATOR)+","+std::to_string(this->frame_rate);
		return text;
	}


}


