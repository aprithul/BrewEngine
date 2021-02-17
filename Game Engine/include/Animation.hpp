#ifndef ANIMATION_HPP
#define ANIMATION_HPP

#include <sstream>
#include <vector>
#include "Texture.hpp"
#include "Vector3.hpp"
namespace PrEngine {

	struct Keyframe
	{
		Vector3<Float_32> position;
		Vector3<Float_32> rotation;
		Vector3<Float_32> scale;
		Uint_32 texture;
		Float_32 timestamp;
	};

	struct Animation
	{
		Animation();
		Animation(std::string& animation_name);
		~Animation();
		std::string clip_name;
		std::vector<Keyframe> frames;
	};
}
#endif