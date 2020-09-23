#include "Animation.hpp"

namespace PrEngine {
	Animation::Animation(std::string& animation_name)
	{
		std::string data = read_file(animation_name);
		// parse aniamtion file
		std::stringstream data_stream(data);
		std::string keyframe_data;

		while (std::getline(data_stream, keyframe_data, '\n')) // get an entity
		{
			std::stringstream keyframe_stream(keyframe_data);
			std::string token;
			std::vector<std::string> tokens;
			while (std::getline(keyframe_stream, token, ','))
			{
				tokens.push_back(token);
			}

			Keyframe keyframe;
			// timestamp
			keyframe.timestamp = std::stof(tokens[0]);

			//position
			keyframe.position.x = std::stof(tokens[1]);
			keyframe.position.y = std::stof(tokens[2]);
			keyframe.position.z = std::stof(tokens[3]);

			//scale
			keyframe.scale.x = std::stof(tokens[4]);
			keyframe.scale.y = std::stof(tokens[5]);
			keyframe.scale.z = std::stof(tokens[6]);
			
			//scale
			keyframe.rotation.x = std::stof(tokens[7]);
			keyframe.rotation.y = std::stof(tokens[8]);
			keyframe.rotation.z = std::stof(tokens[9]);

			//texture
			keyframe.texture = Texture::load_texture(tokens[10]);

			frames.push_back(keyframe);
		}

	}

	Animation::~Animation()
	{

	}
}