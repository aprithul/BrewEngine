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
#include <stdlib.h>

namespace PrEngine{
	class EntityGenerator
	{
	public:
		EntityGenerator(RendererOpenGL2D* renderer);
		~EntityGenerator();
		Entity* make_sprite_entity(const std::string& image_file_path);
		Entity* make_light_entity();

	private:
		RendererOpenGL2D* renderer;
	};

}
#endif /* INCLUDE_ENTITYGENERATOR_HPP_ */
