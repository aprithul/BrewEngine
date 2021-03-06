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

	std::unordered_map<std::string, Animation> Animator::animations_library;

	Animator::Animator():Component(COMP_ANIMATOR)
	{
		animation_speed = 1.f;
		current_frame_index = 0;
	}

	Animator::~Animator()
	{

	}

	void Animator::start()
	{
		
	}

	
	void Animator::update()
	{
		static Float_32 frame_time = 0;
		frame_time += Time::Frame_time;
		Keyframe frame = animation.frames[current_frame_index];
		
		if (frame.timestamp <= frame_time * animation_speed)
		{
			//transform->translate(frame.position);
			
			//LOG(LOGTYPE_GENERAL, "Anim id: " + std::to_string(id_transform));
			get_transform(id_transform).position = frame.position;
			get_transform(id_transform).scale = frame.scale;
			get_transform(id_transform).rotation = frame.rotation;
			transform_dirty_flag[id_transform] = true;
			//set_valid(transform_dirty_flag, id_transform);

			//Material* mat = Material::get_material(graphics[id_graphic].element.material);
			//mat->diffuse_textures[0] = frame.texture;
			current_frame_index = (current_frame_index+1)%((Int_32)(animation.frames.size()));
			frame_time = 0;
		}
	}

	Bool_8 Animator::load_animation(std::string& file_name)
	{
		animations_library.emplace(file_name, file_name);
		if(!Animation::animation_load_status)
			LOG(LOGTYPE_ERROR, file_name, " : animation loading failed");

		return Animation::animation_load_status;
	}

	std::string Animator::to_string()
	{
		std::string text = std::to_string(COMP_ANIMATOR);
		for (std::unordered_map<std::string, Animation>::iterator it = animations_library.begin(); it != animations_library.end(); it++)
		{
			text += "," + it->second.clip_name;
		}
		return text;
	}




}


