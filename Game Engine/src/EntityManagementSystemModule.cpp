#include "EntityManagementSystemModule.hpp"
#include <algorithm>
namespace PrEngine
{
	EntityManagementSystem* entity_management_system = nullptr;

	Transform3D transforms[MAX_ENTITY_COUNT];
	Camera cameras[MAX_CAMERA_COUNT];
	//Sprite sprites[MAX_SPRITE_COUNT];
	Graphic graphics[MAX_GRAPHIC_COUNT];
	DirectionalLight directional_lights[MAX_DIRECTIONAL_LIGHT_COUNT];
	Animator animators[MAX_ANIMATOR_COUNT];
	Bool_8 entity_validity[MAX_ENTITY_COUNT] = {};
	std::unordered_map<ComponentType, Uint_32> entities[MAX_ENTITY_COUNT];
	
	std::unordered_set<Uint_32> transform_children[MAX_ENTITY_COUNT];
	Uint_32 transform_order[MAX_ENTITY_COUNT] = {};
	Uint_32 transform_hierarchy_level[MAX_ENTITY_COUNT] = {};
	Bool_8 transform_dirty_flag[MAX_ENTITY_COUNT] = {};
	//Bool_8 transform_active_status[MAX_ENTITY_COUNT] = {};
	//Bool_8 sprite_active_status[MAX_SPRITE_COUNT] = {};
	//Bool_8 directional_light_active_status[MAX_DIRECTIONAL_LIGHT_COUNT] = {};
	//Bool_8 graphic_active_status[MAX_GRAPHIC_COUNT] = {};
	//Bool_8 camera_active_status[MAX_CAMERA_COUNT] = {};
	//Bool_8 animator_active_status[MAX_ANIMATOR_COUNT] = {};


	std::queue<Uint_32> EntityManagementSystem::released_positions;
	std::queue<Uint_32> EntityManagementSystem::transform_released_positions;
	std::queue<Uint_32> EntityManagementSystem::transform_order_positions;
	std::queue<Uint_32> EntityManagementSystem::sprite_released_positions;
	std::queue<Uint_32> EntityManagementSystem::graphics_released_positions;
	std::queue<Uint_32> EntityManagementSystem::directional_light_released_positions;
	std::queue<Uint_32> EntityManagementSystem::animator_released_positions;
	std::queue<Uint_32> EntityManagementSystem::camera_released_positions;

	Uint_32 EntityManagementSystem::entity_count;
	Uint_32 EntityManagementSystem::next_entity_pos;
	Uint_32 EntityManagementSystem::next_transform_pos;
	Uint_32 EntityManagementSystem::next_transform_order;
	Uint_32 EntityManagementSystem::next_sprite_pos;
	Uint_32 EntityManagementSystem::next_graphic_pos;
	Uint_32 EntityManagementSystem::next_directional_light_pos;
	Uint_32 EntityManagementSystem::next_animator_pos;
	Uint_32 EntityManagementSystem::next_camera_pos;
	Uint_32 EntityManagementSystem::camera_entity_id[MAX_CAMERA_COUNT] = {};
	//Uint_32 EntityManagementSystem::sprite_entity_id[MAX_SPRITE_COUNT]={};
	Uint_32 EntityManagementSystem::graphics_entity_id[MAX_GRAPHIC_COUNT]={};
	Uint_32 EntityManagementSystem::directional_light_entity_id[MAX_DIRECTIONAL_LIGHT_COUNT]={};
	Uint_32 EntityManagementSystem::animator_entity_id[MAX_ANIMATOR_COUNT]={};
	Uint_32 EntityManagementSystem::transform_entity_id[MAX_ENTITY_COUNT]={};

    EntityManagementSystem::EntityManagementSystem(std::string name, Int_32 priority) : Module(name, priority)
    {
		assert(entity_management_system == nullptr);

    	next_entity_pos = 1;
		next_animator_pos = 1;
		next_camera_pos = 1;
		next_directional_light_pos = 1;
		next_graphic_pos = 1;
		next_sprite_pos = 1;
		next_transform_pos = 1;
		next_transform_order = 1;
    	entity_count = 0;

        entity_management_system = this;
    }

    EntityManagementSystem::~EntityManagementSystem()
    {
       //
    }

	char EntityManagementSystem::get_entity(Uint_32 id)
    {
		// needs implementation
		return 0;
    }

	//delete entity through its transform
    void EntityManagementSystem::delete_entity_transform(Uint_32 id_transform)
    {
		if (transform_entity_id[id_transform])
		{
			for (auto it : transform_children[id_transform])
			{
				delete_entity_transform(it);
			}
				
			auto ent_id = transform_entity_id[id_transform];
			auto& ent = entities[ent_id];
			
			entity_count--;
			released_positions.push(ent_id);

			transform_entity_id[id_transform] = 0;
			for (auto& it : transform_order)
				if (it == id_transform)
					it = 0;

			transform_children[id_transform].clear();
			transforms[id_transform].parent_transform = 0;

			Uint_32 c_id{}, s_id{}, g_id{}, l_id{}, a_id{};
			auto it = ent.find(COMP_CAMERA);
			if (it != ent.end())
				c_id = it->second;

			it = ent.find(COMP_SPRITE);
			if (it != ent.end())
				s_id = it->second;

			it = ent.find(COMP_GRAPHICS);
			if (it != ent.end())
				g_id = it->second;

			it = ent.find(COMP_LIGHT);
			if (it != ent.end())
				l_id = it->second;

			it = ent.find(COMP_ANIMATOR);
			if (it != ent.end())
				a_id = it->second;

			if (c_id)
				delete_camera_comp(c_id);
			if (s_id)
				delete_sprite_comp(s_id);
			if (g_id)
				delete_graphic_comp(g_id);
			if (l_id)
				delete_directional_light_comp(l_id);
			if (a_id)
				delete_animator_comp(a_id);

			return;
		}
    }

	Uint_32 EntityManagementSystem::make_entity()
    {
		Uint_32 _id = next_entity_pos;
		if (released_positions.empty() != true)
		{
			_id = released_positions.front();
			released_positions.pop();
		}
		else
			next_entity_pos++;

		entity_count++;
		entity_validity[_id] = true;
		entities[_id].clear();
		return _id;
    }

	Uint_32 EntityManagementSystem::make_transform_comp(Uint_32 entity_id)
	{
		Uint_32 _id = next_transform_pos;
		if (transform_released_positions.empty() != true)
		{
			_id = transform_released_positions.front();
			transform_released_positions.pop();
		}
		else
			next_transform_pos++;

		Uint_32 _order = next_transform_order;
		if (transform_order_positions.empty() != true)
		{
			_order = transform_order_positions.front();
			transform_order_positions.pop();
		}
		else
			next_transform_order++;

		transform_order[_order] = _id;
		transform_entity_id[_id] = entity_id;
		entities[entity_id][COMP_TRANSFORM_3D] = _id;

		//transform_active_status[_id] = true;
		transform_children[_id].clear();
		transform_hierarchy_level[_id] = MAX_HIERARCHY_LEVEL;
		sort_transform_order();

		return _id;
	}

	Uint_32 EntityManagementSystem::make_camera_comp(Uint_32 entity_id)
	{
		Uint_32 _id = next_camera_pos;
		if (camera_released_positions.empty() != true)
		{
			_id = camera_released_positions.front();
			camera_released_positions.pop();
		}
		else
			next_camera_pos++;

		camera_entity_id[_id] = entity_id;
		entities[entity_id][COMP_CAMERA] = _id;

		//camera_active_status[_id] = true;
		return _id;
	}

	Uint_32 EntityManagementSystem::make_animator_comp(Uint_32 entity_id)
	{
		Uint_32 _id = next_animator_pos;
		if (!animator_released_positions.empty())
		{
			_id = animator_released_positions.front();
			animator_released_positions.pop();
		}
		else
			next_animator_pos++;

		animator_entity_id[_id] = entity_id;
		entities[entity_id][COMP_ANIMATOR] = _id;

		//animator_active_status[_id] = true;

		return _id;
	}

	//Uint_32 EntityManagementSystem::make_sprite_comp(Uint_32 entity_id)
	//{
	//	Uint_32 _id = next_sprite_pos;
	//	if (sprite_released_positions.empty() != true)
	//	{
	//		_id = sprite_released_positions.front();
	//		sprite_released_positions.pop();
	//	}
	//	else
	//		next_sprite_pos++;

	//	sprite_entity_id[_id] = entity_id;
	//	entities[entity_id][COMP_SPRITE] = _id;

	//	//sprite_active_status[_id] = true;

	//	return _id;
	//}

	Uint_32 EntityManagementSystem::make_graphic_comp(Uint_32 entity_id)
	{
		Uint_32 _id = next_graphic_pos;
		if (graphics_released_positions.empty() != true)
		{
			_id = graphics_released_positions.front();
			graphics_released_positions.pop();
		}
		else
			next_graphic_pos++;

		graphics_entity_id[_id] = entity_id;
		entities[entity_id][COMP_GRAPHICS] = _id;

		//graphic_active_status[_id] = true;

		return _id;
	}

	Uint_32 EntityManagementSystem::make_directional_light_comp(Uint_32 entity_id)
	{
		Uint_32 _id = next_directional_light_pos;
		if (directional_light_released_positions.empty() != true)
		{
			_id = directional_light_released_positions.front();
			directional_light_released_positions.pop();
		}
		else
			next_directional_light_pos++;

		directional_light_entity_id[_id] = entity_id;
		entities[entity_id][COMP_LIGHT] = _id;

		//directional_light_active_status[_id] = true;

		return _id;
	}
	

	Bool_8 EntityManagementSystem::delete_camera_comp(Uint_32 c_id)
	{
		if (!c_id)
		{
			LOG(LOGTYPE_ERROR, "camera comp : " + std::to_string(c_id) + " not valid, couldn't delete");
			return false;
		}
		entities[camera_entity_id[c_id]][COMP_CAMERA] = 0;
		camera_entity_id[c_id] = 0;
		camera_released_positions.push(c_id);
		return true;

	}
	Bool_8 EntityManagementSystem::delete_graphic_comp(Uint_32 g_id)
	{
		if (!g_id)
		{
			LOG(LOGTYPE_ERROR, "graphics component : " + std::to_string(g_id) + " not valid, couldn't delete");
			return false;
		}
		entities[graphics_entity_id[g_id]][COMP_GRAPHICS] = 0;
		graphics_entity_id[g_id] = 0;
		graphics_released_positions.push(g_id);
		return true;
	}
	Bool_8 EntityManagementSystem::delete_sprite_comp(Uint_32 sprite_id)
	{
		//entities[camera_entity_id[c_id]][COMP_CAMERA] = 0;
		//camera_entity_id[c_id] = 0;
		//camera_released_positions.push(c_id);
		return false;
	}
	Bool_8 EntityManagementSystem::delete_animator_comp(Uint_32 a_id)
	{
		if (!a_id)
		{
			LOG(LOGTYPE_ERROR, "animator component : " + std::to_string(a_id) + " not valid, couldn't delete");
			return false;
		}

		entities[animator_entity_id[a_id]][COMP_ANIMATOR] = 0;
		animator_entity_id[a_id] = 0;
		animator_released_positions.push(a_id);
		return true;
	}
	Bool_8 EntityManagementSystem::delete_directional_light_comp(Uint_32 l_id)
	{
		if (!l_id)
		{
			LOG(LOGTYPE_ERROR, "directional light component : " + std::to_string(l_id) + " not valid, couldn't delete");
			return false;
		}

		entities[directional_light_entity_id[l_id]][COMP_LIGHT] = 0;
		directional_light_entity_id[l_id] = 0;
		directional_light_released_positions.push(l_id);
		return true;
	}
	/*Uint_32 EntityManagementSystem::delete_transform_comp(Uint_32 transform_id)
	{
		for (auto it : transform_children[transform_id])
		{
			delete_entity_transform(it);
		}

		entities[transform_entity_id[transform_id]][COMP_TRANSFORM_3D] = 0;
		transform_entity_id[transform_id] = 0;

		for (auto& it : transform_order)
		{
			if (it == transform_id)
			{
				it = 0;
				break;
			}
		}

		transform_children[transform_id].clear();
		transforms[transform_id].parent_transform = 0;
	}*/


	void EntityManagementSystem::set_parent_transform(Uint_32 parent_transform, Uint_32& child_transform)
	{
		transforms[child_transform].parent_transform = parent_transform;
		transform_children[parent_transform].insert(child_transform);
		decrease_hierarchy_level_recursively(child_transform);
		sort_transform_order();
		return;
	}

	void EntityManagementSystem::decrease_hierarchy_level_recursively(Uint_32 transform)
	{
		transform_hierarchy_level[transform]--;
assert(transform_hierarchy_level > 0);

		for (auto it : transform_children[transform])//.begin(); it != transform_children[transform].end(); it++)
		{
			decrease_hierarchy_level_recursively(it);
		}
	}

	bool comp(Uint_32 i, Uint_32 j)
	{
		auto r = (transform_hierarchy_level[i] > transform_hierarchy_level[j]);
		return r;
	}

	void EntityManagementSystem::sort_transform_order()
	{

		std::sort(transform_order + 1, transform_order + next_transform_order, comp);
		return;
	}

    void EntityManagementSystem::start()
    {

    }

	inline Bool_8 is_valid(Uint32* validity, Uint_32 pos)
	{
		return (validity[pos / 32] & (1 << (pos % 32)));
	}

	inline void set_valid(Uint32* validity, Uint_32 pos)
	{
		validity[pos / 32] = (validity[pos / 32] | (1 << (pos % 32)));
	}

	inline void clear_valid(Uint32* validity, Uint_32 pos)
	{
		validity[pos / 32] = (validity[pos / 32] & ~(1 << (pos % 32)));
	}

    void EntityManagementSystem::update()
    {
		for (Uint_32 i = 1; i < next_transform_order; i++)
		{
			int j = transform_order[i];
			if(transform_entity_id[j])
				transforms[j].update();
		}

		for (Uint_32 i = 0; i < next_camera_pos; i++)
		{
			if (camera_entity_id[i])
				cameras[i].update();
		}

		for (Uint_32 i = 0; i < next_directional_light_pos; i++)
		{
			if (directional_light_entity_id[i])
				directional_lights[i].update();
		}

		for (Uint_32 i = 0; i < next_animator_pos; i++)
		{
			if (animator_entity_id[i])
				animators[i].update();
		}
    }

    void EntityManagementSystem::end()
    {

    }

	void EntityManagementSystem::save_scene(const std::string& scene_file)
	{
		write_to_file("", scene_file, 0, 0); //clears file

		std::unordered_map<int, std::string> entities_in_scene;

		for (Uint_32 i = 1; i < next_transform_order; i++)
		{
			int j = transform_order[i];
			if (transform_entity_id[j])
				entities_in_scene[transform_entity_id[j]] = transforms[j].to_string() + ","+ std::to_string(j) + "\n";
		}

		for (Uint_32 i = 0; i < next_camera_pos; i++)
		{
			if (camera_entity_id[i])
				entities_in_scene[camera_entity_id[i]] += cameras[i].to_string() + "\n";
		}

		for (Uint_32 i = 0; i < next_directional_light_pos; i++)
		{
			if (directional_light_entity_id[i])
				entities_in_scene[directional_light_entity_id[i]] += directional_lights[i].to_string() + "\n";
		}
		for (Uint_32 i = 0; i < next_graphic_pos; i++)
		{
			if (graphics_entity_id[i])
				entities_in_scene[graphics_entity_id[i]] += graphics[i].to_string() + "\n";
		}

		for (Uint_32 i = 0; i < next_animator_pos; i++)
		{
			if (animator_entity_id[i])
				entities_in_scene[animator_entity_id[i]] += animators[i].to_string() + "\n";
		}

		for (auto it : entities_in_scene)
		{
			write_to_file(it.second + "~\n", scene_file, 0, 1);
		}
	}

	Uint_32 EntityManagementSystem::get_active_camera()
	{
		for (int i = 1; i < MAX_CAMERA_COUNT; i++)
		{
			if (camera_entity_id[i])
				return i;
		}
		return 0;
	}
}
