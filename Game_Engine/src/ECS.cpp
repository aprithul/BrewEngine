//#include "ECS.hpp"
//namespace PrEngine
//{
//	template<typename T>
//	ComponentSystem<T>::ComponentSystem(Uint_32 comp_per_ent, Uint_32 system_size)
//	{
//		ent_to_comp_array = new Uint_32[MAX_ENTITY_COUNT*comp_per_ent];
//		components = new T[system_size*comp_per_ent];
//	}
//
//	template<typename T>
//	ComponentSystem<T>::~ComponentSystem()
//	{
//		delete[] ent_to_comp_array;
//		delete[] components;
//	}
//	
//	template<typename T>
//	void ComponentSystem<T>::start()
//	{
//		for (Uint_32 _i =0; _i<new_comp_pos; _i++)
//		{
//			(static_cast<Component*>(&components[_i]))->start();
//		}
//	}
//
//	template<typename T>
//	void ComponentSystem<T>::update()
//	{
//		for (Uint_32 _i = 0; _i < new_comp_pos; _i++)
//		{
//			(static_cast<Component*>(&components[_i]))->update();
//
//		}
//	}
//
//	template<typename T>
//	void ComponentSystem<T>::end()
//	{
//		for (Uint_32 _i = 0; _i < new_comp_pos; _i++)
//		{
//			(static_cast<Component*>(&components[_i]))->end();
//
//		}
//	}
//
//	template<typename T>
//	T* ComponentSystem<T>::get(Uint_32 id)
//	{
//		// 28b entity | 4b offset
//		Uint_32 entity = id >> 4;
//		Uint_32 offset = 0x0000000F & id;
//		Uint_32 index = (entity*comp_per_ent) + offset;
//		if (ent_to_comp_array[index])
//			return &components[ent_to_comp_array[index]];
//		else
//			return nullptr;
//	}
//
//	template<typename T>
//	void ComponentSystem<T>::remove(Uint_32 id)
//	{
//		// 28b entity | 4b offset
//		Uint_32 entity = id >> 4;
//		Uint_32 offset = 0x0000000F & id;
//		Uint_32 index = (entity*comp_per_ent) + offset;
//		if (ent_to_comp_array[index])
//		{
//			components[ent_to_comp_array[index]] = components[new_comp_pos - 1];
//			for (Uint_32 _i = (MAX_ENTITY_COUNT*comp_per_ent)-1; _i >= 0; _i--)
//			{
//				if (ent_to_comp_array[_i] == new_comp_pos - 1)
//				{
//					ent_to_comp_array[_i] = ent_to_comp_array[index];
//					break;
//				}
//			}
//			new_comp_pos--;
//		}
//	}
//
//	template<typename T>
//	Uint_32 ComponentSystem<T>::add(Uint_32 entity)
//	{
//		Uint_32 index = entity * comp_per_ent;
//		Uint_32 id = 0;
//
//		for (Uint_32 _i = index; _i < index + comp_per_ent; _i++)
//		{
//			if (!ent_to_comp_array[_i])
//			{
//				ent_to_comp_array[_i] = new_comp_pos;
//				new_comp_pos++;
//				id = entity << 4 | _i;
//			}
//		}
//		return id;
//	}
//
//
//	ECS::ECS(std::string _name, Int_32 priority):Module(_name, priority)
//	{
//		graphics_component_system = new ComponentSystem<Graphic>(1, MAX_ENTITY_COUNT);
//		batched_graphics_component_system = new ComponentSystem<BatchedGraphic>(1, MAX_BATCH_COUNT);
//		animator_component_system = new ComponentSystem<Animator>(1, MAX_ANIMATOR_COUNT);
//		transform_component_system = new ComponentSystem<Transform3D>(1, MAX_TRANSFORM_COUNT);
//		collider_component_system = new ComponentSystem<Collider>(3, MAX_COLLIDER_COUNT);
//		scripting_component_system = new ComponentSystem<Scripting>(8, MAX_SCRIPT_COUNT);
//		camera_component_system = new ComponentSystem<Camera>(1, MAX_CAMERA_COUNT);
//	}
//
//	void ECS::start()
//	{
//		graphics_component_system->start();
//		batched_graphics_component_system->start();
//		animator_component_system->start();
//		transform_component_system->start();
//		collider_component_system->start();
//		scripting_component_system->start();
//		camera_component_system->start();
//
//	}
//
//	void ECS::update()
//	{
//		graphics_component_system->update();
//		batched_graphics_component_system->update();
//		animator_component_system->update();
//		transform_component_system->update();
//		collider_component_system->update();
//		scripting_component_system->update();
//		camera_component_system->update();
//	}
//
//	void ECS::end()
//	{
//		batched_graphics_component_system->end();
//		animator_component_system->end();
//		transform_component_system->end();
//		collider_component_system->end();
//		scripting_component_system->end();
//		camera_component_system->end();
//
//	}
//}