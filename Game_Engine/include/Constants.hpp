#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP
#include "Types.hpp"

namespace PrEngine
{
	static const Uint_32 Max_entity_count = 500;
	static const Uint_32 Max_system_component_id = Max_entity_count;
	static const Uint_32 Max_transform_count = Max_entity_count;
	static const Uint_32 Max_graphics_count = Max_entity_count;
	static const Uint_32 Max_batched_graphics_count = 200;
	static const Uint_32 Max_animator_count = Max_entity_count;
	static const Uint_32 Max_animation_count = Max_animator_count * 3;
	static const Uint_32 Max_camera_count = 1;
	static const Uint_32 Max_scripting_count = Max_entity_count * 16;
	static const Uint_32 Max_collider_count = Max_entity_count * 2;
	static const Uint_32 Max_rigidbody2d_count = Max_entity_count;
}

#endif