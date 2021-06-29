#ifndef ECS_HPP
#define ECS_HPP

#include "Module.hpp"
#include "Component.hpp"
#include "Graphics.hpp"
#include "Animator.hpp"
#include "Transform3D.hpp"
#include "Collider.hpp"
#include "ScriptComponent.hpp"
#include "Camera3D.hpp"

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
	template <typename T>
	struct ComponentSystem
	{
		ComponentSystem(Uint_32 comp_per_ent, Uint_32 system_size);
		~ComponentSystem();

		void start();
		void update();
		void end();

		T* get(Uint_32 id);
		void remove(Uint_32 id);
		Uint_32 add(Uint_32 entity);

	private:
		//components shouldn't be assignable/copyable
		ComponentSystem(const ComponentSystem&) = delete;
		ComponentSystem(ComponentSystem&&) = delete;
		ComponentSystem& operator=(const ComponentSystem&) = delete;
		ComponentSystem& operator=(const ComponentSystem&&) = delete;

		// reference to the components array
		T* components;
		Uint_32 comp_per_ent;
		Uint_32* ent_to_comp_array;
		
		// 
		Uint_32 new_comp_pos = 1;


	};

	struct ECS : public Module
	{
		// component systems
		ComponentSystem<Graphic>* graphics_component_system;
		ComponentSystem<BatchedGraphic>* batched_graphics_component_system;
		ComponentSystem<Animator>* animator_component_system;
		ComponentSystem<Transform3D>* transform_component_system;
		ComponentSystem<Collider>* collider_component_system;
		ComponentSystem<Scripting>* scripting_component_system;
		ComponentSystem<Camera>* camera_component_system;


		ECS(std::string _name, Int_32 priority);
		void start() override;
		void update() override;
		void end() override;

	};
}


#endif // !ECS_HPP


