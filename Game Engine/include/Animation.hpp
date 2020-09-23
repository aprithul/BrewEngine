#ifndef ANIMATION_HPP
#define ANIMATION_HPP

#include <sstream>
#include <vector>
#include "Texture.hpp"
#include "Vector3.hpp"
namespace PrEngine {

	struct Keyframe
	{
		Vector3<float> position;
		Vector3<float> rotation;
		Vector3<float> scale;
		Texture* texture;
		float timestamp;

	};

	class Animation
	{
	public:
		Animation(std::string& animation_name);
		~Animation();

	private:
		std::vector<Keyframe> frames;
	};
}
#endif