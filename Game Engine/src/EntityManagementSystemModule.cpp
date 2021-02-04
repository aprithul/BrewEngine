#include "EntityManagementSystemModule.hpp"
#include <algorithm>
namespace PrEngine
{
	EntityManagementSystem* entity_management_system = nullptr;

	Transform3D transforms[MAX_ENTITY_COUNT];
	Camera cameras[MAX_CAMERA_COUNT];
	Sprite sprites[MAX_SPRITE_COUNT];
	Graphic graphics[MAX_GRAPHIC_COUNT];
	DirectionalLight directional_lights[MAX_DIRECTIONAL_LIGHT_COUNT];
	Animator animators[MAX_ANIMATOR_COUNT];
	Bool_8 entity_validity[MAX_ENTITY_COUNT] = {};

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
	std::queue<Uint_32> EntityManagementSystem::graphic_released_positions;
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
	Uint_32 EntityManagementSystem::sprite_entity_id[MAX_SPRITE_COUNT]={};
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

	//needs total rework. Should be huge bottleneck with a lots of entities.
    void EntityManagementSystem::delete_entity(Uint_32 id)
    {
		if (entity_validity[id])
		{
			LOG(LOGTYPE_ERROR, "Requested entity couldn't be found for deleting : ", std::to_string(id));
		}
		else
		{
			entity_validity[id] = false;
			// disable all components
			for (int _i = 0; _i < MAX_TRANSFORM_COUNT; _i++)
			{
				if (transform_entity_id[_i] == id) {
					transform_entity_id[_i] = 0;
					//transform_active_status[_i] = false;
					break;
				}
			}
			
			if (camera_entity_id[1] == id)
				camera_entity_id[1] = 0;
				//camera_active_status[1] = false;

			for (int _i = 0; _i < MAX_SPRITE_COUNT; _i++)
			{
				if (sprite_entity_id[_i] == id) {
					sprite_entity_id[_i] = 0;
					//sprite_active_status[_i] = false;
					break;
				}
			}

			for (int _i = 0; _i < MAX_GRAPHIC_COUNT; _i++)
			{
				if (graphics_entity_id[_i] == id)
				{
					graphics_entity_id[_i] = 0;
					//graphic_active_status[_i] = false;
					break;
				}

			}

			for (int _i = 0; _i < MAX_ANIMATOR_COUNT; _i++)
			{
				if (animator_entity_id[_i] == id) {
					animator_entity_id[_i] = 0;
					//animator_active_status[_i] = false;
					break;
				}
			}

			if (directional_light_entity_id[0] = id)
				directional_light_entity_id[0] = 0;
				//directional_light_active_status[0] = false;;

			entity_count--;
			released_positions.push(id);
			LOG(LOGTYPE_WARNING, "Entity deleted : ", std::to_string(id));

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
		//animator_active_status[_id] = true;

		return _id;
	}

	Uint_32 EntityManagementSystem::make_sprite_comp(Uint_32 entity_id)
	{
		Uint_32 _id = next_sprite_pos;
		if (sprite_released_positions.empty() != true)
		{
			_id = sprite_released_positions.front();
			sprite_released_positions.pop();
		}
		else
			next_sprite_pos++;

		sprite_entity_id[_id] = entity_id;
		//sprite_active_status[_id] = true;

		return _id;
	}

	Uint_32 EntityManagementSystem::make_graphic_comp(Uint_32 entity_id)
	{
		Uint_32 _id = next_graphic_pos;
		if (graphic_released_positions.empty() != true)
		{
			_id = graphic_released_positions.front();
			graphic_released_positions.pop();
		}
		else
			next_graphic_pos++;

		graphics_entity_id[_id] = entity_id;
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
		//directional_light_active_status[_id] = true;

		return _id;
	}
	
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

		for (auto it = transform_children[transform].begin(); it != transform_children[transform].end(); it++)
		{
			decrease_hierarchy_level_recursively(*it);
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
