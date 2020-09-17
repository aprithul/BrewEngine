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
#include <vector>
namespace PrEngine{
	class Animator : public Component
	{
	public:
		Animator(Material* material);
		~Animator();
		int current_frame_index;
		void play();
		void pause();
		void stop();
		void set_frame_rate(int frame_rate);
		void update() override;
		void add_frame(Texture* frame);

	private:
		void on_next_frame();
		std::vector<Texture*> animation_frames;
		double frame_delta;
		int frame_rate;
		Timer* on_frame_change_timer;
		Material* material;
	};
}


#endif /* INCLUDE_ANIMATOR_HPP_ */
