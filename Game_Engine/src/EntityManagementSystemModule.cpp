#include "EntityManagementSystemModule.hpp"
#include "PhysicsModule.hpp"
#include <algorithm>
#include <time.h>
namespace PrEngine
{
	EntityManagementSystem* entity_management_system = nullptr;
	std::vector<std::string> script_names;

	Transform3D transforms[MAX_ENTITY_COUNT];
	std::vector<Uint_32> transform_children[MAX_ENTITY_COUNT];

	Camera cameras[MAX_CAMERA_COUNT];
	Graphic graphics[MAX_GRAPHIC_COUNT];
	BatchedGraphic batched_graphics[MAX_BATCH_COUNT];
	DirectionalLight directional_lights[MAX_DIRECTIONAL_LIGHT_COUNT];
	Collider colliders[MAX_COLLIDER_COUNT];
	Animator animators[MAX_ANIMATOR_COUNT];
	Bool_8 entity_validity[MAX_ENTITY_COUNT] = {};
	Scripting scripting_comps[MAX_SCRIPT_COUNT];
	std::unordered_map<ComponentType, Uint_32> entities[MAX_ENTITY_COUNT];
	
	Uint_32 camera_entity_id[MAX_CAMERA_COUNT] = {};
	Uint_32 graphics_entity_id[MAX_GRAPHIC_COUNT] = {};
	Uint_32 directional_light_entity_id[MAX_DIRECTIONAL_LIGHT_COUNT] = {};
	Uint_32 animator_entity_id[MAX_ANIMATOR_COUNT] = {};
	Uint_32 collider_entity_id[MAX_COLLIDER_COUNT] = {};
	Uint_32 scripting_entity_id[MAX_SCRIPT_COUNT] = {};
	Uint_32 transform_entity_id[MAX_ENTITY_COUNT] = {};

	/*Uint_32 transform_order[MAX_ENTITY_COUNT] = {};
	Uint_32 transform_hierarchy_level[MAX_ENTITY_COUNT] = {};
	Bool_8 transform_dirty_flag[MAX_ENTITY_COUNT] = {};*/

	Uint_32 entity_count;
	Uint_32 next_entity_pos;
	Uint_32 next_transform_pos;
	Uint_32 next_sprite_pos;
	Uint_32 next_graphic_pos;
	Uint_32 next_batched_graphic_pos;
	Uint_32 next_directional_light_pos;
	Uint_32 next_animator_pos;
	Uint_32 next_collider_pos;
	Uint_32 next_camera_pos;
	Uint_32 next_scripting_pos;


	std::queue<Uint_32> EntityManagementSystem::released_positions;
	std::queue<Uint_32> EntityManagementSystem::transform_released_positions;
	std::queue<Uint_32> EntityManagementSystem::transform_order_positions;
	std::queue<Uint_32> EntityManagementSystem::sprite_released_positions;
	std::queue<Uint_32> EntityManagementSystem::graphics_released_positions;
	std::queue<Uint_32> EntityManagementSystem::directional_light_released_positions;
	std::queue<Uint_32> EntityManagementSystem::animator_released_positions;
	std::queue<Uint_32> EntityManagementSystem::collider_released_positions;
	std::queue<Uint_32> EntityManagementSystem::camera_released_positions;
	std::queue<Uint_32> EntityManagementSystem::script_released_positions;


    EntityManagementSystem::EntityManagementSystem(std::string name, Int_32 priority) : Module(name, priority)
    {
		assert(entity_management_system == nullptr);

    	next_entity_pos = 1;
		next_animator_pos = 1;
		next_camera_pos = 1;
		next_directional_light_pos = 1;
		next_graphic_pos = 1;
		next_batched_graphic_pos = 1;
		next_sprite_pos = 1;
		next_transform_pos = 1;
		next_collider_pos = 1;
		next_scripting_pos = 1;
		entity_count = 0;
        entity_management_system = this;
		
		// load all script file names
		std::string file_content = read_file(get_resource_path() + PATH_SEP + "type_names.csv");
		script_names.push_back(""); // empty at 0
		tokenize_string(file_content, ',', script_names);
    }

    EntityManagementSystem::~EntityManagementSystem()
    {

    }

	char EntityManagementSystem::get_entity(Uint_32 id)
    {
		// needs implementation
		return 0;
    }

	//delete entity through its transform
    void EntityManagementSystem::delete_entity_by_transform(Uint_32 id_transform)
    {
		/*if (transform_entity_id[id_transform])
		{
			for (auto it : transform_children[id_transform])
			{
				delete_entity_by_transform(it);
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

			Uint_32 c_id{}, s_id{}, g_id{}, l_id{}, a_id{}, col_id{}, scr_id{};
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
			it = ent.find(COMP_COLLIDER);
			if (it != ent.end())
				col_id = it->second;

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
		}*/
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

		transform_entity_id[_id] = entity_id;
		entities[entity_id][COMP_TRANSFORM_3D] = _id;

		transform_children[_id].clear();
		transforms[_id].children = &transform_children[_id];

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

	Uint_32 EntityManagementSystem::make_collider_comp(Uint_32 entity_id)
	{
		Uint_32 _id = next_collider_pos;
		if (collider_released_positions.empty() != true)
		{
			_id = collider_released_positions.front();
			collider_released_positions.pop();
		}
		else
			next_collider_pos++;

		collider_entity_id[_id] = entity_id;
		entities[entity_id][COMP_COLLIDER] = _id;

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

	Uint_32 EntityManagementSystem::make_scripting_comp(Uint_32 entity_id)
	{
		Uint_32 _id = next_scripting_pos;
		if (!script_released_positions.empty())
		{
			_id = script_released_positions.front();
			script_released_positions.pop();
		}
		else
			next_scripting_pos++;

		scripting_entity_id[_id] = entity_id;
		entities[entity_id][COMP_SCRIPT] = _id;

		return _id;
	}

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
	Bool_8 EntityManagementSystem::delete_collider_comp(Uint_32 col_id)
	{
		if (!col_id)
		{
			LOG(LOGTYPE_ERROR, "Collider comp : " + std::to_string(col_id) + " not valid, couldn't delete");
			return false;
		}
		entities[collider_entity_id[col_id]][COMP_COLLIDER] = 0;
		collider_entity_id[col_id] = 0;
		collider_released_positions.push(col_id);
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
		graphics[g_id].Delete();
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


	void EntityManagementSystem::set_parent_transform(Uint_32 parent_transform, Uint_32 child_transform)
	{
		Uint_32 current_parent = transforms[child_transform].parent_transform;
		if (current_parent)
		{
			std::vector<Uint_32>& children = *transforms[current_parent].children;
			for (Uint_32 _i = 0; _i < children.size(); _i++)
				if (children[_i] == child_transform)
					children[_i] = 0;
		}

		Point3d glob_pos = transforms[child_transform].get_global_position();
		Quaternion glob_rot = transforms[child_transform].get_global_rotation();
		//Vec3f glob_scl = transforms[child_transform].get_global_scale();

		transforms[child_transform].parent_transform = parent_transform;
		if(parent_transform)	
			transforms[parent_transform].children->push_back(child_transform);

		transforms[child_transform].set_global_position(glob_pos);
		transforms[child_transform].set_global_rotation(glob_rot);

	}

    void EntityManagementSystem::start()
    {

//		Animator::animations_library.emplace_back(_empty_animation_name);
//		Animator::animation_clip_names.push_back(_empty_animation_name);
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

		//clock_t begin = clock();
		//update_transforms();
		//clock_t end = clock();

		//Double_64 elapsed = (Double_64)(end - begin) / CLOCKS_PER_SEC;

		for (Uint_32 i = 0; i < next_transform_pos; i++)
		{
			if(transform_entity_id[i])
				if (transforms[i].parent_transform == 0) // better to move parent_transform outside ?
					transforms[i].update();
		}
		
		//begin = clock();
		for (Uint_32 i = 0; i < next_camera_pos; i++)
		{
			if (camera_entity_id[i])
				cameras[i].update();
		}
		//end = clock();
		//elapsed = (Double_64)(end - begin) / CLOCKS_PER_SEC;

		//begin = clock();
		for (Uint_32 i = 0; i < next_directional_light_pos; i++)
		{
			if (directional_light_entity_id[i])
				directional_lights[i].update();
		}
		//end = clock();
		//elapsed = (Double_64)(end - begin) / CLOCKS_PER_SEC;

		//begin = clock();
		for (Uint_32 i = 0; i < next_animator_pos; i++)
		{
			if (animator_entity_id[i])
				animators[i].update();
		}

		//begin = clock();
		for (Uint_32 i = 0; i < next_scripting_pos; i++)
		{
			if (scripting_entity_id[i])
				scripting_comps[i].update();
		}


		clock_t begin = clock();

		// find collision
		for (Uint_32 i = 0; i < next_collider_pos; i++)
		{
			if (collider_entity_id[i])
			{
				Uint_32 t_id_i = colliders[i].transform_id;
				if (!t_id_i)continue;
				//Rect<Float_32> r1 = points_to_rect_with_transform(colliders[i].collision_shape.points, transforms[t_id_i].transformation);

				for (Uint_32 j = i+1; j < next_collider_pos; j++)
				{
					if (collider_entity_id[j])
					{
						Uint_32 t_id_j = colliders[j].transform_id;
						if (!t_id_j)continue;
						//Rect<Float_32> r2 = points_to_rect_with_transform(colliders[j].collision_shape.points, transforms[t_id_j].transformation);
						
						//clock_t begin = clock();
						Bool_8 did_intersect = intersect_GJK(colliders[i], colliders[j]);
						//clock_t end = clock();
						//Double_64 elapsed = (Double_64)(end - begin) / CLOCKS_PER_SEC;

						if (did_intersect)
						{
							//clock_t begin = clock();
							Vec2f col_pen_vec = do_EPA(colliders[i], colliders[j]);
							//clock_t end = clock();
							//Double_64 elapsed = (Double_64)(end - begin) / CLOCKS_PER_SEC;

							physics_module->contacts.push_back(Contact{ col_pen_vec, i, j });
						}
/*
						if (intersect_AABB_AABB(r1, r2))
						{
							physics_module->contacts.push_back(Contact{ i, j });
						}*/
					}
				}

			}
		}
		clock_t end = clock();
		Double_64 elapsed = (Double_64)(end - begin) / CLOCKS_PER_SEC;
		//LOG(LOGTYPE_GENERAL, std::to_string(elapsed));
		return;
    }

    void EntityManagementSystem::end()
    {

    }

	void EntityManagementSystem::save_scene(const std::string& scene_file)
	{

		write_to_file("", scene_file, 0, 0); //clears file
		std::unordered_map<int, std::string> entities_in_scene;

		for (Uint_32 i = 1; i < next_transform_pos; i++)
		{
			//int j = transform_order[i];
			if (transform_entity_id[i])
				entities_in_scene[transform_entity_id[i]] = transforms[i].to_string() + ","+ std::to_string(i) + "\n";
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
		for (Uint_32 i = 0; i < next_animator_pos; i++)
		{
			if (animator_entity_id[i])
				entities_in_scene[animator_entity_id[i]] += animators[i].to_string() + "\n";
		}

		for (Uint_32 i = 0; i < next_graphic_pos; i++)
		{
			if (graphics_entity_id[i])
			{
				auto ed = Graphic::editor_data[i];
				entities_in_scene[graphics_entity_id[i]] += graphics[i].to_string() + "," + std::to_string(ed.future_tag)
					+ "," + std::to_string(ed.scale) + "\n";
			}
		}

		for (Uint_32 i = 0; i < next_collider_pos; i++)
		{
			if (collider_entity_id[i])
				entities_in_scene[collider_entity_id[i]] += colliders[i].to_string() + "\n";
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

	Uint_32 EntityManagementSystem::add_script_to_entity(Uint_32 entity, Script* script, Uint_32 name_index)
	{
		Uint_32 scripting_id = entities[entity][COMP_SCRIPT];
		if (!scripting_id)
		{
			scripting_id = entity_management_system->make_scripting_comp(entity);
		}
		Scripting& scripting = scripting_comps[entities[entity][COMP_SCRIPT]];
		script->entity = entity;
		return scripting.add_script(script, name_index);
	}
}
