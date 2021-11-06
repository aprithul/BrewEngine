/*
 * Animator.cpp
 *
 *  Created on: Sep 17, 2020
 *      Author: cobre
 */

#include "Animator.hpp"
#include "EntityManagementSystemModule.hpp"
#include "Graphics.hpp"
#include <mutex>

namespace PrEngine
{

	std::vector<Animation> Animator::animations_library = { Animation() };
	std::vector<std::string> Animator::animation_clip_names = { "NULL ANIM" };

	Animator::Animator():Component(COMP_ANIMATOR)
	{
		initialize();
	}

	Animator::~Animator()
	{

	}

	void Animator::initialize()
	{
		animation_speed = 1.f;
		id_transform = 0;
		std::memset(animation_ids, 0, sizeof(Uint_32) * 8);
		animation_count = 0;
		cur_anim_ind = 0;
		current_frame_index = 0;
		std::memset(anim_transform_update_flags, 0, sizeof(Bool_8) * 3);
		frame_time = 0;
	}

	void Animator::start()
	{

	}

	
	void Animator::update(Float_32 dt)
	{
		//frame_time = 0;
		frame_time += dt;
		Animation& animation = get_current_animation();
		if(animation.frames.size() > current_frame_index)
		{		
			Keyframe frame = animation.frames[current_frame_index];
			Transform3D& transformation = transform_system.get_component(id_transform);
			if (frame.timestamp <= frame_time * animation_speed)
			{
				//transform->translate(frame.position);
				if(anim_transform_update_flags[ANIM_TRANSLATE])
					transformation.set_local_position( frame.position.x, frame.position.y, frame.position.z);

				if (anim_transform_update_flags[ANIM_ROTATE])
				{

					transformation.set_local_rotation(frame.rotation.x, frame.rotation.y, frame.rotation.z);
					//rotation = Mat4x4::Identity();
					//rotation(0, 0) = cosf(b) * cosf(c);
					//rotation(0, 1) = cosf(b) * sinf(c);
					//rotation(0, 2) = -sinf(b);

					//rotation(1, 0) = (sinf(a) * sinf(b) * cosf(c)) - (cosf(a) * sinf(c));
					//rotation(1, 1) = (sinf(a) * sinf(b) * sinf(c)) + (cosf(a) * cosf(c));
					//rotation(1, 2) = sinf(a)*cosf(b);

					//rotation(2, 0) = (cosf(a) * sinf(b) * cosf(c)) + (sinf(a) * sinf(c));
					//rotation(2, 1) = (cosf(a) * sinf(b) * sinf(c)) - (sinf(a) * cosf(c));
					//rotation(2, 2) = cosf(a) * cosf(b);
					//rotation = Quaternion::EulerToQuaternion(Vec3f{ a,b,c });
				}

				//scale = Mat3x3::Identity();
				if (anim_transform_update_flags[ANIM_SCALE])
				{
					transformation.set_local_scale(frame.scale.x, frame.scale.y, frame.scale.z);
				}


				//LOG(LOGTYPE_GENERAL, "Anim id: " + std::to_string(id_transform));
				//transforms[id_transform].position = frame.position;
				//transforms[id_transform].scale = frame.scale;
				//transforms[id_transform].rotation = frame.rotation;
				//transform_dirty_flag[id_transform] = true;
				//set_valid(transform_dirty_flag, id_transform);

				//Material* mat = Material::get_material(graphics[id_graphic].element.material);
				//mat->diffuse_textures[0] = frame.texture;
				current_frame_index = (current_frame_index + 1) % ((Int_32)(animation.frames.size()));
				frame_time = 0;
			}
		}
	}

	void Animator::end()
	{

	}
	std::mutex add_animation_mutex;
	void Animator::add_animation(Uint_32 animation_id)
	{
		add_animation_mutex.lock();
		for (Uint_32 a : animation_ids)
		{
			if (a == animation_id)
				return;
		}
		
		if (animation_count < MAX_ANIMATIONS)
		{
			animation_ids[animation_count] = animation_id;
			animation_count++;
		}
		else
			LOG(LOGTYPE_ERROR, "Max number of animations already added to animator");

		add_animation_mutex.unlock();
	}

	std::mutex animation_loader_mutex;
	Uint_32 Animator::load_animation(const std::string& file_name)
	{
		animation_loader_mutex.lock();
			Animation::animation_load_status = 1;
			Int_32 present_at = -1;
			for (int _i = 0; _i < animation_clip_names.size(); _i++)
			{
				if (animation_clip_names[_i] == file_name)
				{
					present_at = _i;
					break;
				}
			}

			Uint_32 animation_pos = present_at;
			if (present_at == -1)
			{
				animations_library.emplace_back(file_name);
				animation_clip_names.push_back(file_name);
				animation_pos = animations_library.size() - 1;
			}
		animation_loader_mutex.unlock();

		return animation_pos;
	}

	std::string Animator::to_string()
	{
		std::string text = std::to_string(COMP_ANIMATOR);
		text += ","+std::to_string(cur_anim_ind)+","+std::to_string(anim_transform_update_flags[0])+"," + std::to_string(anim_transform_update_flags[1])+"," + std::to_string(anim_transform_update_flags[2]);
		for (int _i=0; _i < MAX_ANIMATIONS ; _i++)
		{
			if(animation_ids[_i])
				text += ","+ animation_clip_names[animation_ids[_i]];
		}
		return text;
	}
}


