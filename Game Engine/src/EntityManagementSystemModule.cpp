#include "EntityManagementSystemModule.hpp"

namespace PrEngine
{
    EntityManagementSystem* entity_management_system;
	
	Uint_32 transform_active_status[(int)(1.f + (MAX_ENTITY_COUNT / 32.f))];
	Uint_32 transform_entity_id[MAX_ENTITY_COUNT];
	Transform3D transforms[MAX_ENTITY_COUNT];

	Uint_32 camera_active_status[1];
	Uint_32 camera_entity_id[1];
	Camera cameras[1];
	
	Uint_32 sprite_active_status[1 + (MAX_SPRITE_COUNT / 32)];
	Uint_32 sprite_entity_id[MAX_SPRITE_COUNT];
	Sprite sprites[MAX_SPRITE_COUNT];

	Uint_32 graphic_active_status[1 + (MAX_GRAPHIC_COUNT / 32)];
	Uint_32 graphic_entity_id[MAX_GRAPHIC_COUNT];
	Graphic graphics[MAX_GRAPHIC_COUNT];

	Uint_32 directional_light_active_status[1];
	Uint_32 directional_light_entity_id[1];
	DirectionalLight directional_lights[1];

	Uint_32 animator_active_status[1 + (MAX_ANIMATOR_COUNT / 32)];
	Uint_32 animator_entity_id[MAX_ANIMATOR_COUNT];
	Animator animators[MAX_ANIMATOR_COUNT];

	Uint_32 entity_validity[MAX_ENTITY_COUNT / 32];

    EntityManagementSystem::EntityManagementSystem(std::string name, Int_32 priority) : Module(name, priority)
    {
    	next_entity_pos = 1;
		next_animator_pos = 0;
		next_camera_pos = 0;
		next_directional_light_pos = 0;
		next_graphic_pos = 0;
		next_sprite_pos = 0;
		next_transform_pos = 0;

    	entity_count = 0;
        for(Uint_32 i=0;i<MAX_ENTITY_COUNT/32;i++)
            entity_validity[i] = 0;
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

    void EntityManagementSystem::delete_entity(Uint_32 id)
    {
		if (!is_valid(entity_validity, id))
		{
			LOG(LOGTYPE_ERROR, "Requested entity couldn't be found for deleting : ", std::to_string(id));
		}
		else
		{
			clear_valid(entity_validity, id);
			// disable all components
			for (int _i = 0; _i < MAX_TRANSFORM_COUNT; _i++)
			{
				if (transform_entity_id[_i] == id) {
					clear_valid(transform_active_status, _i);
					break;
				}
			}
			
			if (camera_entity_id[0] == id)
				clear_valid(camera_active_status, 0);

			for (int _i = 0; _i < MAX_SPRITE_COUNT; _i++)
			{
				if (sprite_entity_id[_i] == id) {
					clear_valid(sprite_active_status, _i);
					break;
				}
			}

			for (int _i = 0; _i < MAX_GRAPHIC_COUNT; _i++)
			{
				if (graphic_entity_id[_i] == id)
				{
					clear_valid(graphic_active_status, _i);
					break;
				}

			}

			for (int _i = 0; _i < MAX_ANIMATOR_COUNT; _i++)
			{
				if (animator_entity_id[_i] == id) {
					clear_valid(animator_active_status, _i);
					break;
				}
			}

			if(directional_light_entity_id[0] = id)
				clear_valid(directional_light_active_status, 0);

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
		set_valid(entity_validity,_id);
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

		transform_entity_id[_id] = entity_id;
		set_valid(transform_active_status, _id);
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
		set_valid(camera_active_status, _id);
		return _id;
	}

	Uint_32 EntityManagementSystem::make_animator_comp(Uint_32 entity_id)
	{
		Uint_32 _id = next_animator_pos;
		if (animator_released_positions.empty() != true)
		{
			_id = animator_released_positions.front();
			animator_released_positions.pop();
		}
		else
			next_animator_pos++;

		animator_entity_id[_id] = entity_id;
		set_valid(animator_active_status, _id);

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
		set_valid(sprite_active_status, _id);

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

		graphic_entity_id[_id] = entity_id;
		set_valid(graphic_active_status, _id);

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
		set_valid(directional_light_active_status, _id);

		return _id;
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
		for (Uint_32 i = 0; i < next_transform_pos; i++)
		{
			if(is_valid(transform_active_status, i))
				transforms[i].update();
		}

		for (Uint_32 i = 0; i < next_camera_pos; i++)
		{
			if (is_valid(camera_active_status, i))
				cameras[i].update();
		}

		for (Uint_32 i = 0; i < next_directional_light_pos; i++)
		{
			if (is_valid(directional_light_active_status, i))
				directional_lights[i].update();
		}

		for (Uint_32 i = 0; i < next_animator_pos; i++)
		{
			if (is_valid(animator_active_status, i))
				animators[i].update();
		}
    }

    void EntityManagementSystem::end()
    {

    }

}
