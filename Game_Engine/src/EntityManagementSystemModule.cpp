#include "EntityManagementSystemModule.hpp"
#include "PhysicsModule.hpp"
#include <algorithm>
#include <time.h>
namespace PrEngine
{
	EntityManagementSystem* entity_management_system = nullptr;
	std::vector<std::string> script_names;

	ComponentSystem<Camera> camera_system(Max_camera_count);
	ComponentSystem<Graphic> graphics_system(Max_graphics_count);
	ComponentSystem<BatchedGraphic> batched_graphics_system(Max_batched_graphics_count);
	ComponentSystem<Animator> animator_system(Max_animator_count);
	ComponentSystem<Collider> collider_system(Max_collider_count);
	ComponentSystem<Scripting> scripting_system(Max_scripting_count);
	TransformSystem transform_system(Max_transform_count);

    EntityManagementSystem::EntityManagementSystem(std::string name, Int_32 priority) : Module(name, priority)
    {
		assert(entity_management_system == nullptr);
        entity_management_system = this;
		
		// load all script file names
		std::string file_content = read_file(get_resource_path() + PATH_SEP + "type_names.csv");
		script_names.push_back(""); // empty at 0
		tokenize_string(file_content, ',', script_names);
    }

    EntityManagementSystem::~EntityManagementSystem()
    {

    }

	//delete entity through its transform
    void EntityManagementSystem::delete_entity_by_transform(Uint_32 id_transform)
    {
		Uint_32 entity = transform_system.get_entity(id_transform);
		transform_system.remove(id_transform);

		Uint_32 graphics_id = graphics_system.get_component_id(entity);
		if (graphics_id)
		{
			graphics_system.remove(graphics_id);
		}

		Uint_32 animator_id = animator_system.get_component_id(entity);
		if (animator_id)
		{
			animator_system.remove(animator_id);
		}

		Uint_32 camera_id = camera_system.get_component_id(entity);
		if (camera_id)
		{
			camera_system.remove(camera_id);
		}

		Uint_32 scripting_id = scripting_system.get_component_id(entity);
		if (scripting_id)
		{
			scripting_system.remove(scripting_id);
		}

		Uint_32 collider_id = collider_system.get_component_id(entity);
		if (collider_id)
		{
			collider_system.remove(collider_id);
		}

    }

	Uint_32 EntityManagementSystem::make_entity()
    {
		Uint_32 entity = new_entity_pos;
		if (released_entity_positions.empty() != true)
		{
			entity = released_entity_positions.front();
			released_entity_positions.pop();
		}

		if (entity <= Max_entity_count)
		{
			new_entity_pos++;
			return entity;
		}
		else
			return 0;
    }


	void EntityManagementSystem::set_parent_transform(Uint_32 parent_transform, Uint_32 child_transform)
	{
		Uint_32 current_parent = transform_system.get_component(child_transform).parent_transform;
		if (current_parent)
		{
			std::vector<Uint_32>& children = *transform_system.get_component(current_parent).children;
			for (Uint_32 _i = 0; _i < children.size(); _i++)
				if (children[_i] == child_transform)
					children[_i] = 0;
		}

		Point3d glob_pos = transform_system.get_component(child_transform).get_global_position();
		Quaternion glob_rot = transform_system.get_component(child_transform).get_global_rotation();

		transform_system.get_component(child_transform).parent_transform = parent_transform;
		if(parent_transform)	
			transform_system.get_component(parent_transform).children->push_back(child_transform);

		transform_system.get_component(child_transform).set_global_position(glob_pos);
		transform_system.get_component(child_transform).set_global_rotation(glob_rot);

	}

    void EntityManagementSystem::start()
    {
		;
    }


    void EntityManagementSystem::update()
    {
		transform_system.update();
		animator_system.update();
		scripting_system.update();
		camera_system.update();

		// find collision
//		for (Uint_32 i = 0; i < next_collider_pos; i++)
//		{
//			if (collider_entity_id[i])
//			{
//				Uint_32 t_id_i = colliders[i].transform_id;
//				if (!t_id_i)continue;
//				//Rect<Float_32> r1 = points_to_rect_with_transform(colliders[i].collision_shape.points, transforms[t_id_i].transformation);
//
//				for (Uint_32 j = i+1; j < next_collider_pos; j++)
//				{
//					if (collider_entity_id[j])
//					{
//						Uint_32 t_id_j = colliders[j].transform_id;
//						if (!t_id_j)continue;
//						//Rect<Float_32> r2 = points_to_rect_with_transform(colliders[j].collision_shape.points, transforms[t_id_j].transformation);
//						
//						//clock_t begin = clock();
//						Bool_8 did_intersect = intersect_GJK(colliders[i], colliders[j]);
//						//clock_t end = clock();
//						//Double_64 elapsed = (Double_64)(end - begin) / CLOCKS_PER_SEC;
//
//						if (did_intersect)
//						{
//							//clock_t begin = clock();
//							Vec2f col_pen_vec = do_EPA(colliders[i], colliders[j]);
//							//clock_t end = clock();
//							//Double_64 elapsed = (Double_64)(end - begin) / CLOCKS_PER_SEC;
//
//							physics_module->contacts.push_back(Contact{ col_pen_vec, i, j });
//						}
///*
//						if (intersect_AABB_AABB(r1, r2))
//						{
//							physics_module->contacts.push_back(Contact{ i, j });
//						}*/
//					}
//				}
//
//			}
//		}
		return;
    }

    void EntityManagementSystem::end()
    {

    }

	void EntityManagementSystem::save_scene(const std::string& scene_file)
	{
		/*
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
		}*/
	}

	Uint_32 EntityManagementSystem::get_active_camera()
	{
		return 1;	// temporary
	}

	Uint_32 EntityManagementSystem::add_script_to_entity(Uint_32 entity, Script* script, Uint_32 name_index)
	{
		Uint_32 scripting_id = scripting_system.get_component_id(entity);// entities[entity][COMP_SCRIPT];
		if (!scripting_id)
		{
			scripting_id = scripting_system.make(entity);// entity_management_system->make_scripting_comp(entity);
		}
		Scripting& scripting = scripting_system.get_component(scripting_id);// scripting_comps[entities[entity][COMP_SCRIPT]];
		script->entity = entity;
		return scripting.add_script(script, name_index);
	}






}
