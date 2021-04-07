#ifndef ANIMATION_HPP
#define ANIMATION_HPP

#include <sstream>
#include <vector>
#include "Texture.hpp"
#include "Vec.hpp"
namespace PrEngine {

	struct Keyframe // 44 Bytes
	{
		Vec3f position;
		Vec3f rotation;
		Vec3f scale;
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