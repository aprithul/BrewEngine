#ifndef	COMPONENT_SYSTEM_HPP
#define COMPONENT_SYSTEM_HPP

#include "Types.hpp"
#include "Constants.hpp"
#include "Transform3D.hpp"

namespace PrEngine {
	template <typename T>
	struct ComponentSystem
	{

		ComponentSystem(Uint_32 size);
		~ComponentSystem();

		Uint_32 new_pos;
		Uint_32 new_id;

		virtual Uint_32 make(Uint_32 entity);
		Bool_8 remove(Uint_32 id);
		Uint_32 get_entity(Uint_32 comp_id);
		Uint_32 get_component_id(Uint_32 entity);
		T& get_component(Uint_32 comp_id);
		const T* get_comp_array();

		void start();
		void update();
		void end();

	protected:
		T* components;
		Uint_32* id_to_comp_pos;
		//std::queue<Uint_32> released;
		Uint_32* comp_id_to_entity;
		Uint_32* entity_to_comp_id;

		Uint_32 system_size;
	};

	struct TransformSystem :public ComponentSystem<Transform3D>
	{
		std::vector<Uint_32> transform_children[Max_entity_count + 1];
		Uint_32 make(Uint_32 entity) override
		{
			if (new_pos <= system_size)
			{
				Uint_32 _id = new_id;
				id_to_comp_pos[_id] = new_pos;
				reinterpret_cast<Transform3D*>(&components[new_pos])->initialize();

				comp_id_to_entity[_id] = entity;
				entity_to_comp_id[entity] = _id;

				transform_children[new_pos].clear();
				reinterpret_cast<Transform3D*>(&components[new_pos])->children = &transform_children[new_pos];
				//reinterpret_cast<Component*>(&components[new_pos])->start();

				new_id++;
				new_pos++;

				return _id;
			}
			else
				return 0;
		}

		TransformSystem(Uint_32 size) :ComponentSystem<Transform3D>(size)
		{

		}
	};

	template <typename T>
	ComponentSystem<T>::ComponentSystem(Uint_32 size) :new_pos(1), new_id(1)
	{
		components = new T[size + 1];	// +1 so that we can start from index 1 and keep 0 for null

		id_to_comp_pos = new Uint_32[Max_system_component_id + 1];
		memset(id_to_comp_pos, 0, sizeof(Uint_32)*(Max_system_component_id + 1));

		comp_id_to_entity = new Uint_32[size + 1];
		memset(comp_id_to_entity, 0, sizeof(Uint_32)*(size + 1));

		entity_to_comp_id = new Uint_32[Max_entity_count + 1];
		memset(entity_to_comp_id, 0, sizeof(Uint_32)*(Max_entity_count + 1));

		system_size = size;
	}

	template <typename T>
	ComponentSystem<T>::~ComponentSystem()
	{
		delete[] components;
		delete[] id_to_comp_pos;
		delete[] comp_id_to_entity;
		delete[] entity_to_comp_id;
	}

	template <typename T>
	Uint_32 ComponentSystem<T>::make(Uint_32 entity)
	{
		if (new_pos <= system_size)
		{
			Uint_32 _id = new_id;
			id_to_comp_pos[_id] = new_pos;

			reinterpret_cast<Component*>(&components[new_pos])->initialize();

			comp_id_to_entity[_id] = entity;
			entity_to_comp_id[entity] = _id;


			new_id++;
			new_pos++;

			return _id;
		}
		else
			return 0;

	}

	template <typename T>
	Bool_8 ComponentSystem<T>::remove(Uint_32 comp_id)
	{
		if (!comp_id)
		{
			LOG(LOGTYPE_ERROR, "Component ID: " + std::to_string(comp_id) + " not valid, couldn't be deleted");
			return false;
		}

		Uint_32 comp_pos = id_to_comp_pos[comp_id];
		id_to_comp_pos[comp_id] = 0;
		components[comp_pos] = components[new_pos - 1];



		entity_to_comp_id[comp_id_to_entity[comp_id]] = 0;
		comp_id_to_entity[comp_id] = 0;

		for (Uint_32 _i = new_id - 1; _i >= 1; _i--)
		{
			if (id_to_comp_pos[_i] == (new_pos - 1))
			{
				id_to_comp_pos[_i] = comp_pos;
				break;
			}
		}

		new_pos--;

		return true;
	}

	template <typename T>
	Uint_32 ComponentSystem<T>::get_entity(Uint_32 comp_id)
	{
		return comp_id_to_entity[comp_id];
	}

	template <typename T>
	Uint_32 ComponentSystem<T>::get_component_id(Uint_32 entity)
	{
		return entity_to_comp_id[entity];
	}

	template <typename T>
	const T* ComponentSystem<T>::get_comp_array()
	{
		return components;
	}


	template <typename T>
	T& ComponentSystem<T>::get_component(Uint_32 comp_id)
	{
		Uint_32 comp_pos = id_to_comp_pos[comp_id];
		return components[comp_pos];
	}

	template <typename T>
	void ComponentSystem<T>::start()
	{
		for (Uint_32 _i = 1; _i < new_pos; _i++)
		{
			components[_i].start();
			//(reinterpret_cast<T>(components[_i])).start();
		}
	}

	template <typename T>
	void ComponentSystem<T>::update()
	{
		for (Uint_32 _i = 1; _i < new_pos; _i++)
		{
			//reinterpret_cast<T*>(&components[_i])->update();
			components[_i].update();

		}
	}

	template <typename T>
	void ComponentSystem<T>::end()
	{
		for (Uint_32 _i = 1; _i < new_pos; _i++)
		{
			components[_i].end();
			//(reinterpret_cast<T>(components[_i])).end();

		}
	}
}
#endif