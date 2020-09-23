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
	class Animator : public Component
	{
	public:
		Animator();
		~Animator();
		int current_frame_index;
		float animation_speed;
		void load_animation(std::string& animation_file);

		void start() override;
		void play();
		void pause();
		void stop();
		void update() override;
		std::string to_string() override;
	private:
		Material* material;
		Transform3D* transform;
		std::unordered_map<std::string, Animation> aniamtions;
	};
}


#endif /* INCLUDE_ANIMATOR_HPP_ */
