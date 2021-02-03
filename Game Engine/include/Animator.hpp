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
namespace PrEngine{
	struct Animator : public Component
	{
		Animator();
		~Animator();
		Int_32 current_frame_index;
		Animation current_animation;
		Float_32 animation_speed;
		Float_32 animator_time;
		Uint_32 id_transform;
		Uint_32 id_graphic;

		void start() override;
		void play();
		void pause();
		void stop();
		void update() override;
		std::string to_string() override;

		static void load_animation(std::string& animation_file);
		static std::unordered_map<std::string, Animation> animations;

		
	};
}


#endif /* INCLUDE_ANIMATOR_HPP_ */
