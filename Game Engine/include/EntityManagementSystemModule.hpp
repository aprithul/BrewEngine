#ifndef ENTITY_MANAGEMENT_SYSTEM_HPP
#define ENTITY_MANAGEMENT_SYSTEM_HPP

#include "Module.hpp"
#include "Entity.hpp"
#include "Camera3D.hpp"
#include "Animator.hpp"
#include "Transform3D.hpp"
#include "Sprite.hpp"
#include "DirectionalLight.hpp"
#include "Types.hpp"
#include "Logger.hpp"
#include <unordered_map>
#include <vector>
#include <queue>
#include <climits>
#define MAX_ENTITY_COUNT 99

namespace PrEngine
{


	// components
	extern Transform3D transforms[MAX_ENTITY_COUNT];
	extern Uint_32 transform_active_status[MAX_ENTITY_COUNT / 32];

	extern Camera cameras[1];
	extern Uint_32 camera_active_status[MAX_ENTITY_COUNT / 32];

	extern Sprite sprites[MAX_ENTITY_COUNT];
	extern Uint_32 sprite_active_status[MAX_ENTITY_COUNT / 32];

	extern Graphic graphics[MAX_ENTITY_COUNT];
	extern Uint_32 graphic_active_status[MAX_ENTITY_COUNT / 32];

	extern DirectionalLight directional_lights[1];
	extern Uint_32 directional_light_active_status[MAX_ENTITY_COUNT / 32];

	extern Animator animators[MAX_ENTITY_COUNT];
	extern Uint_32 animator_active_status[MAX_ENTITY_COUNT / 32];

	extern Uint_32 entity_validity[MAX_ENTITY_COUNT / 32];

	inline Bool_8 is_valid(Uint32* validity, Uint_32 pos);
	inline void set_valid(Uint32* validity, Uint_32 pos);
	inline void clear_valid(Uint32* validity, Uint_32 pos);

    struct EntityManagementSystem : public Module
    {
		Uint_32 entity_count;
		Uint_32 next_entity_pos;
		std::queue<Uint_32> released_positions;

		Uint_32 next_transform_pos;
		std::queue<Uint_32> transform_released_positions;
		Uint_32 next_sprite_pos;
		std::queue<Uint_32> sprite_released_positions;
		Uint_32 next_graphic_pos;
		std::queue<Uint_32> graphic_released_positions;
		Uint_32 next_directional_light_pos;
		std::queue<Uint_32> directional_light_released_positions;
		Uint_32 next_animator_pos;
		std::queue<Uint_32> animator_released_positions;
		Uint_32 next_camera_pos;
		std::queue<Uint_32> camera_released_positions;

		EntityManagementSystem(std::string name, Int_32 priority);
		~EntityManagementSystem();

		char get_entity(Uint_32 id);
		void delete_entity(Uint_32 id);
		Uint_32 make_entity();
		Uint_32 make_camera_comp(Uint_32 entity_id);
		Uint_32 make_graphic_comp(Uint_32 entity_id);
		Uint_32 make_sprite_comp(Uint_32 entity_id);
		Uint_32 make_animator_comp(Uint_32 entity_id);
		Uint_32 make_directional_light_comp(Uint_32 entity_id);
		Uint_32 make_transform_comp(Uint_32 entity_id);

            
		void start() override;
		void update() override;
		void end() override;
    };
	
	extern EntityManagementSystem* entity_management_system;

}

#endif