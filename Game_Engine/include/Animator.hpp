/*
 * Animator.hpp
 *
 *  Created on: Sep 17, 2020
 *      Author: cobre
 */

#ifndef INCLUDE_ANIMATOR_HPP_
#define INCLUDE_ANIMATOR_HPP_

#include "Entity.hpp"
#include "Material.hpp"
#include "Texture.hpp"
#include "TimeModule.hpp"
#include "Serializable.hpp"
#include "Transform3D.hpp"
#include "Animation.hpp"
#include <string>
#include <unordered_map>

#define MAX_ANIMATIONS 8

namespace PrEngine{

	enum ANIM_FLAGS
	{
		ANIM_TRANSLATE,
		ANIM_ROTATE,
		ANIM_SCALE,

		ANIM_COUNT
	};

	struct Animator : public Component
	{
		Animator();
		~Animator();

		//Mat4x4 translation;// = Matrix4x4<Float_32>::identity();
		//Quaternion rotation;// = Matrix4x4<Float_32>::identity();
		//Mat3x3 scale;// = Matrix4x4<Float_32>::identity();
		Float_32 animation_speed;
		Uint_32 id_transform;
		Uint_32 animation_ids[8]; // we can hold a max of 8 animations
		Uint_32 animation_count;
		Int_32 cur_anim_ind;	// the current active animation
		Int_32 current_frame_index;
		Bool_8 anim_transform_update_flags[3] = {};	// which of the transform properties to update ( pos, rot, scale )
		Float_32 frame_time;
		//Uint_32 id_graphic;

		void initialize() override;
		void start() override;
		void update(Float_32 dt) override;
		void end() override;

		void play();
		void pause();
		void stop();
		std::string to_string() override;
		void add_animation(Uint_32 animtion_id);
		inline Animation& get_current_animation()
		{
			return animations_library[animation_ids[cur_anim_ind]];
		}

		inline Animation& get_animation(Int_32 anim_id)
		{
			return animations_library[anim_id];
		}

		static Uint_32 load_animation(const std::string& animation_file);
		static std::vector<Animation> animations_library;
		static std::vector<std::string> animation_clip_names;
	};

}


#endif