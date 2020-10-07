/*
 * EntityGenerator.hpp
 *
 *  Created on: Sep 15, 2020
 *      Author: cobre
 */

#ifndef INCLUDE_ENTITYGENERATOR_HPP_
#define INCLUDE_ENTITYGENERATOR_HPP_

#include "Engine.hpp"
#include "EntityManagementSystemModule.hpp"
#include "RendererOpenGL2D.hpp"
#include "Graphics.hpp"
#include "Transform3D.hpp"
#include "Sprite.hpp"
#include <stdlib.h>
#include <sstream>
#include <string>

namespace PrEngine{

	enum EntityTypes
	{
		ANIMATED_SPRITE,
		LIGHT,
		COUNT
	};
	class EntityGenerator
	{
	public:
		EntityGenerator();
		~EntityGenerator();
		Uint_32 make_animated_sprite_entity(const  std::string& image_file_path);
		Uint_32 make_light_entity();
		Uint_32 make_camera();
		void load_scenegraph(std::string& scene_file_name);


	private:
		RendererOpenGL2D* renderer;
	};

}
#endif /* INCLUDE_ENTITYGENERATOR_HPP_ */
