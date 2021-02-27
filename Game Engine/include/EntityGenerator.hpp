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

	class EntityGenerator
	{
	public:
		EntityGenerator();
		Uint_32 make_graphics_entity(const std::string& material_name);
		Uint_32 make_animated_sprite_entity(const  std::string& material_name);
		Uint_32 make_light_entity();
		Uint_32 make_camera_orthographic(Uint_32 width, Uint_32 height);
		void load_scenegraph(const std::string& scene_file_name);


	private:
		static std::unordered_map<int, int> transform_id_mapping;
	};

}
#endif /* INCLUDE_ENTITYGENERATOR_HPP_ */
