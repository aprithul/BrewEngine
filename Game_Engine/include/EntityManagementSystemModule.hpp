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
#include "Constants.hpp"
#include "ComponentSystem.hpp"
#include <unordered_map>
#include <vector>
#include <queue>
#include <climits>
#include <unordered_set>
#include <unordered_map>

namespace PrEngine
{

	extern ComponentSystem<Camera> camera_system;
	extern ComponentSystem<Graphic> graphics_system;
	extern ComponentSystem<BatchedGraphic> batched_graphics_system;
	extern ComponentSystem<Animator> animator_system;
	extern ComponentSystem<Scripting> scripting_system;
	extern TransformSystem transform_system;


    struct EntityManagementSystem : public Module
    {
		static std::string entity_names[Max_entity_count];
		EntityManagementSystem(std::string name, Int_32 priority);
		~EntityManagementSystem();

		void delete_entity_by_transform(Uint_32 id);
		Uint_32 make_entity(const std::string& entity_name);

		void set_parent_transform(Uint_32 parent_transform, Uint_32 child_transform);
		Uint_32 get_active_camera();
		void save_scene(const std::string& scene_file);
		Uint_32 add_script_to_entity(Uint_32 entity, Script* script, Uint_32 name_index);

		void start() override;
		void update() override;
		void end() override;

	private:
		Uint_32 new_entity_pos = 1;
		std::queue<Uint_32> released_entity_positions;

    };
	


	extern EntityManagementSystem* entity_management_system;

}

#endif