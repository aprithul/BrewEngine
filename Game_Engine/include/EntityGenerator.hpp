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
		static std::vector<Uint_32> batched_texture_ids;

		EntityGenerator();
		Uint_32 make_sprite(Uint_32 entity, Float_32 import_scale, RenderTag render_tag, Uint_32 animator_id, Uint_32 transform_id, const std::string& material_name);
		Uint_32 make_sprite(const  std::string& material_path, Point3d position, RenderTag render_tag);
		Uint_32 make_animated_sprite(Point3d position, const std::string& animation_path, const std::string& material_path);
		Uint_32 make_light_entity();
		Uint_32 make_camera_orthographic(Float_32 width, Float_32 height, Float_32 _near, Float_32 _far);
		void load_scenegraph(const std::string& scene_file_name);
		void generate_batches();


	private:
		static std::unordered_map<int, int> transform_id_mapping;
		//std::vector<Uint_32> static_batched_graphic_ids;
	};

}
#endif /* INCLUDE_ENTITYGENERATOR_HPP_ */
