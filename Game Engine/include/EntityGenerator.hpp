/*
 * EntityGenerator.hpp
 *
 *  Created on: Sep 15, 2020
 *      Author: cobre
 */

#ifndef INCLUDE_ENTITYGENERATOR_HPP_
#define INCLUDE_ENTITYGENERATOR_HPP_

#include "EntityManagementSystemModule.hpp"
#include "RendererOpenGL2D.hpp"
#include "Graphics.hpp"
#include "Transform3D.hpp"
#include "Sprite.hpp"
#include <stdlib.h>

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
		EntityGenerator(RendererOpenGL2D* renderer);
		~EntityGenerator();
		Entity* make_animated_sprite_entity(const std::vector<std::string>& image_file_paths,int fps);
		Entity* make_light_entity();

	private:
		RendererOpenGL2D* renderer;
	};

}
#endif /* INCLUDE_ENTITYGENERATOR_HPP_ */
