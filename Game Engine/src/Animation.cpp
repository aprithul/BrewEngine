#include "Animation.hpp"

namespace PrEngine {

	Animation::Animation() 
	{

	}

	Animation::Animation(std::string& animation_name)
	{
		this->clip_name = std::string(animation_name);
		std::string data = read_file(get_resource_path(clip_name));
		// parse aniamtion file
		std::stringstream data_stream(data);
		std::string keyframe_data;

		LOG(LOGTYPE_GENERAL, get_resource_path(clip_name));

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
			LOG(LOGTYPE_GENERAL, "loaded timestamp");
			//position
			keyframe.position.x = std::stof(tokens[1]);
			keyframe.position.y = std::stof(tokens[2]);
			keyframe.position.z = std::stof(tokens[3]);
			LOG(LOGTYPE_GENERAL, "loaded position");

			//scale
			keyframe.scale.x = std::stof(tokens[4]);
			keyframe.scale.y = std::stof(tokens[5]);
			keyframe.scale.z = std::stof(tokens[6]);
			LOG(LOGTYPE_GENERAL, "loaded scale");

			//rotation
			keyframe.rotation.x = std::stof(tokens[7]);
			keyframe.rotation.y = std::stof(tokens[8]);
			keyframe.rotation.z = std::stof(tokens[9]);
			LOG(LOGTYPE_GENERAL, "loaded rotation");

			//texture
			keyframe.texture = Texture::load_texture(tokens[10]);
			if (!Texture::texture_create_status)
				LOG(LOGTYPE_ERROR, "Couldn't load texture : ", tokens[10]);

			frames.push_back(keyframe);
		}

	}

	Animation::~Animation()
	{

	}



}