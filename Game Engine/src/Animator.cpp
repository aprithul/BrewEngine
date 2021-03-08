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
			
			translation = Matrix4x4<Float_32>::identity();
			if (animation.anim_flags[ANIM_TRANSLATE])
			{
				translation.set(0, 3, frame.position.x);
				translation.set(1, 3, frame.position.y);
				translation.set(2, 3, frame.position.z);
			}

			rotation = Matrix4x4<Float_32>::identity();
			if (animation.anim_flags[ANIM_ROTATE])
			{
				Float_32 a = frame.rotation.x * PI / 180.f;
				Float_32 b = frame.rotation.y * PI / 180.f;
				Float_32 c = frame.rotation.z * PI / 180.f;

				rotation.data[(0 * 4) + 0] = cosf(b) * cosf(c);
				rotation.data[(0 * 4) + 1] = cosf(b) * sinf(c);
				rotation.data[(0 * 4) + 2] = -sinf(b);
				rotation.data[(0 * 4) + 3] = 0;

				rotation.data[(1 * 4) + 0] = (sinf(a) * sinf(b) * cosf(c)) - (cosf(a) * sinf(c));
				rotation.data[(1 * 4) + 1] = (sinf(a) * sinf(b) * sinf(c)) + (cosf(a) * cosf(c));
				rotation.data[(1 * 4) + 2] = sinf(a)*cosf(b);
				rotation.data[(1 * 4) + 3] = 0;

				rotation.data[(2 * 4) + 0] = (cosf(a) * sinf(b) * cosf(c)) + (sinf(a) * sinf(c));
				rotation.data[(2 * 4) + 1] = (cosf(a) * sinf(b) * sinf(c)) - (sinf(a) * cosf(c));
				rotation.data[(2 * 4) + 2] = cosf(a) * cosf(b);
				rotation.data[(2 * 4) + 3] = 0;

				rotation.data[(3 * 4) + 0] = 0;
				rotation.data[(3 * 4) + 1] = 0;
				rotation.data[(3 * 4) + 2] = 0;
				rotation.data[(3 * 4) + 3] = 1;
			}

			scale = Matrix4x4<Float_32>::identity();
			if (animation.anim_flags[ANIM_SCALE])
			{
				scale.set(0, 0, frame.scale.x);
				scale.set(1, 1, frame.scale.y);
				scale.set(2, 2, frame.scale.z);
			}


			//LOG(LOGTYPE_GENERAL, "Anim id: " + std::to_string(id_transform));
			//transforms[id_transform].position = frame.position;
			//transforms[id_transform].scale = frame.scale;
			//transforms[id_transform].rotation = frame.rotation;
			//transform_dirty_flag[id_transform] = true;
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


