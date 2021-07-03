#ifndef ENTITY_MANAGEMENT_SYSTEM_HPP
#define ENTITY_MANAGEMENT_SYSTEM_HPP

#include "Platform.hpp"
#include "Module.hpp"
#include "Entity.hpp"
#include "Camera3D.hpp"
#include "Animator.hpp"
#include "Transform3D.hpp"
#include "Sprite.hpp"
#include "Collider.hpp"
#include "DirectionalLight.hpp"
#include "ScriptComponent.hpp"
#include "Logger.hpp"
#include <unordered_map>
#include <vector>
#include <queue>
#include <climits>
#include <unordered_set>
#include <unordered_map>

#define MAX_ENTITY_COUNT 500
#define MAX_TRANSFORM_COUNT MAX_ENTITY_COUNT
#define MAX_GRAPHIC_COUNT MAX_ENTITY_COUNT
#define MAX_BATCH_COUNT 100
#define MAX_COLLIDER_COUNT MAX_ENTITY_COUNT
//#define MAX_SPRITE_COUNT MAX_ENTITY_COUNT/2
#define MAX_ANIMATOR_COUNT MAX_GRAPHIC_COUNT
#define MAX_CAMERA_COUNT 2
#define MAX_SCRIPT_COUNT MAX_ENTITY_COUNT*2
#define MAX_DIRECTIONAL_LIGHT_COUNT 2
#define MAX_HIERARCHY_LEVEL 16


namespace PrEngine
{
	extern std::unordered_map<ComponentType, Uint_32> entities[MAX_ENTITY_COUNT];

	//extern Bool_8 transform_dirty_flag[MAX_ENTITY_COUNT];

	//extern Uint_32 transform_order[MAX_ENTITY_COUNT]; //indices are ids. Indirection allows transforms to be updated in correct order 
	//extern Uint_32 transform_hierarchy_level[MAX_ENTITY_COUNT];
	//extern std::unordered_set<Uint_32> transform_children[MAX_ENTITY_COUNT];

	// component arrays

	extern Transform3D transforms[MAX_ENTITY_COUNT];
	extern std::vector<Uint_32> transform_children[MAX_ENTITY_COUNT];

	extern Camera cameras[MAX_CAMERA_COUNT];
	extern Graphic graphics[MAX_GRAPHIC_COUNT];
	extern BatchedGraphic batched_graphics[MAX_BATCH_COUNT];
	extern DirectionalLight directional_lights[MAX_DIRECTIONAL_LIGHT_COUNT];
	extern Animator animators[MAX_ANIMATOR_COUNT];
	extern Collider colliders[MAX_COLLIDER_COUNT];
	extern Scripting scripting_comps[MAX_SCRIPT_COUNT];
	extern Bool_8 entity_validity[MAX_ENTITY_COUNT];
	extern Uint_32 camera_entity_id[MAX_CAMERA_COUNT];
	extern Uint_32 graphics_entity_id[MAX_GRAPHIC_COUNT];
	extern Uint_32 directional_light_entity_id[MAX_DIRECTIONAL_LIGHT_COUNT];
	extern Uint_32 animator_entity_id[MAX_ANIMATOR_COUNT];
	extern Uint_32 collider_entity_id[MAX_COLLIDER_COUNT];
	extern Uint_32 transform_entity_id[MAX_ENTITY_COUNT];
	extern Uint_32 scripting_entity_id[MAX_SCRIPT_COUNT];


	extern Uint_32 entity_count;
	extern Uint_32 next_scripting_pos;
	extern Uint_32 next_entity_pos;
	extern Uint_32 next_transform_pos;
	extern Uint_32 next_sprite_pos;
	extern Uint_32 next_graphic_pos;
	extern Uint_32 next_batched_graphic_pos;
	extern Uint_32 next_directional_light_pos;
	extern Uint_32 next_animator_pos;
	extern Uint_32 next_collider_pos;
	extern Uint_32 next_camera_pos;


	inline Bool_8 is_valid(Uint32* validity, Uint_32 pos);
	inline void set_valid(Uint32* validity, Uint_32 pos);
	inline void clear_valid(Uint32* validity, Uint_32 pos);


	inline Transform3D& get_transform(Uint_32 entity_id)
	{
		return transforms[entity_id];
		//return transforms[transform_order[entity_id]];
	}


    struct EntityManagementSystem : public Module
    {
		
		static std::queue<Uint_32> released_positions;
		static std::queue<Uint_32> script_released_positions;
		static std::queue<Uint_32> transform_released_positions;
		static std::queue<Uint_32> transform_order_positions;
		static std::queue<Uint_32> sprite_released_positions;
		static std::queue<Uint_32> graphics_released_positions;
		static std::queue<Uint_32> directional_light_released_positions;
		static std::queue<Uint_32> animator_released_positions;
		static std::queue<Uint_32> collider_released_positions;
		static std::queue<Uint_32> camera_released_positions;






		EntityManagementSystem(std::string name, Int_32 priority);
		~EntityManagementSystem();

		char get_entity(Uint_32 id);
		void delete_entity_by_transform(Uint_32 id);
		Uint_32 make_entity();
		Uint_32 make_camera_comp(Uint_32 entity_id);
		Uint_32 make_graphic_comp(Uint_32 entity_id);
		//Uint_32 make_sprite_comp(Uint_32 entity_id);
		Uint_32 make_animator_comp(Uint_32 entity_id);
		Uint_32 make_directional_light_comp(Uint_32 entity_id);
		Uint_32 make_transform_comp(Uint_32 entity_id);
		Uint_32 make_collider_comp(Uint_32 entity_id);
		Uint_32 make_scripting_comp(Uint_32 entity_id);

		Bool_8 delete_camera_comp(Uint_32 camera_id);
		Bool_8 delete_graphic_comp(Uint_32 graphics_id);
		Bool_8 delete_sprite_comp(Uint_32 sprite_id);
		Bool_8 delete_animator_comp(Uint_32 animator_id);
		Bool_8 delete_directional_light_comp(Uint_32 light_id);
		Bool_8 delete_collider_comp(Uint_32 collider_id);
		Bool_8 delete_script_comp(Uint_32 collider_id);

		//Uint_32 delete_transform_comp(Uint_32 transform_id);

		void set_parent_transform(Uint_32 parent_transform, Uint_32 child_transform);
		/*void change_hierarchy_level_recursively(Uint_32 transform, Int_32 level);
		void sort_transform_order();*/
		Uint_32 get_active_camera();
		void save_scene(const std::string& scene_file);
		Uint_32 add_script_to_entity(Uint_32 entity, Script* script, Uint_32 name_index);	/*inline void update_transforms()
		{
			for (Uint_32 i = 1; i < next_transform_order; i++)
			{
				int j = transform_order[i];
				if (transform_entity_id[j])
					transforms[j].update();
			}
		}*/

		void start() override;
		void update() override;
		void end() override;
    };
	


	extern EntityManagementSystem* entity_management_system;

}

#endif