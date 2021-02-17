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

	std::unordered_map<std::string, Animation> Animator::animations;

	Animator::Animator():Component(COMP_ANIMATOR)
	{
		animation_speed = 1.f;
		current_frame_index = 0;
		animator_time = 0;
	}

	Animator::~Animator()
	{

	}

	void Animator::start()
	{
		
	}

	
	void Animator::update()
	{
		animator_time += Time::Frame_time;
		Keyframe frame = current_animation.frames[current_frame_index];
		if (frame.timestamp <= animator_time * animation_speed)
		{
			//transform->translate(frame.position);
			
			//LOG(LOGTYPE_GENERAL, "Anim id: " + std::to_string(id_transform));
			get_transform(id_transform).position = frame.position;
			get_transform(id_transform).scale = frame.scale;
			get_transform(id_transform).rotation = frame.rotation;
			transform_dirty_flag[id_transform] = true;
			//set_valid(transform_dirty_flag, id_transform);

			Material* mat = Material::get_material(graphics[id_graphic].element.material);
			mat->diffuse_texture = frame.texture;
			current_frame_index = (current_frame_index+1)%((Int_32)(current_animation.frames.size()));
			
			// if we've looped around restart timer
			if (current_frame_index == 0)
			{
				animator_time = 0;
			}
		}
	}

	void Animator::load_animation(std::string& file_name)
	{
		animations.emplace(file_name, file_name);
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


