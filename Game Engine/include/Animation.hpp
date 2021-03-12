#ifndef ANIMATION_HPP
#define ANIMATION_HPP

#include <sstream>
#include <vector>
#include "Texture.hpp"
#include "Vector3.hpp"
namespace PrEngine {

	struct Keyframe // 44 Bytes
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
		std::vector<Keyframe> frames;

		static Bool_8 animation_load_status;
	};
}
#endif