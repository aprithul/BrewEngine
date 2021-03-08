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
		Animation animation;

		Matrix4x4<Float_32> translation;// = Matrix4x4<Float_32>::identity();
		Matrix4x4<Float_32> rotation;// = Matrix4x4<Float_32>::identity();
		Matrix4x4<Float_32> scale;// = Matrix4x4<Float_32>::identity();

		Int_32 current_frame_index;
		Float_32 animation_speed;
		Uint_32 id_transform;

		//Uint_32 id_graphic;

		void start() override;
		void play();
		void pause();
		void stop();
		void update() override;
		std::string to_string() override;

		static Bool_8 load_animation(std::string& animation_file);
		static std::unordered_map<std::string, Animation> animations_library;
	};

}


#endif