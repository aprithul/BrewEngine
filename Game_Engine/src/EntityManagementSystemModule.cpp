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
	ComponentSystem<Scripting> scripting_system(Max_scripting_count);
	TransformSystem transform_system(Max_transform_count);

	std::string EntityManagementSystem::entity_names[Max_entity_count + 1];

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
		assert(transform_system.get_entity(id_transform) == 0);
		assert(transform_system.get_component_id(entity) == 0);

		Uint_32 graphics_id = graphics_system.get_component_id(entity);
		if (graphics_id)
		{
			graphics_system.remove(graphics_id);
			assert(graphics_system.get_entity(graphics_id) == 0);
			assert(graphics_system.get_component_id(entity) == 0);

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

		Uint_32 collider_id = PhysicsModule::collider_system.get_component_id(entity);
		if (collider_id)
		{
			PhysicsModule::collider_system.remove(collider_id);
		}

		Uint_32 rigidbody_id = PhysicsModule::rigidbody2d_system.get_component_id(entity);
		if (rigidbody_id)
		{
			PhysicsModule::rigidbody2d_system.remove(rigidbody_id);
		}

		released_entity_positions.push(entity);

    }

	Uint_32 EntityManagementSystem::make_entity(const std::string& entity_name)
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
			entity_names[entity] = entity_name;
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
		transform_system.start();
		animator_system.start();
		scripting_system.start();
		camera_system.start();
    }


    void EntityManagementSystem::update()
    {
		transform_system.update(Time::Frame_time);
		animator_system.update(Time::Frame_time);
		scripting_system.update(Time::Frame_time);
		camera_system.update(Time::Frame_time);

		//find collision
		
		return;
    }

    void EntityManagementSystem::end()
    {
		transform_system.end();
		animator_system.end();
		scripting_system.end();
		camera_system.end();
    }

	void EntityManagementSystem::save_scene(const std::string& scene_file)
	{
		
		std::unordered_map<int, std::string> entities_in_scene;

		for (Uint_32 i = 1; i < transform_system.new_id; i++)
		{
			Uint_32 transform_entity = transform_system.get_entity(i);
			if (transform_entity)
			{
				entities_in_scene[transform_entity] = 
				entities_in_scene[transform_entity] = entity_names[transform_entity] + "\n" + transform_system.get_component(i).to_string() + "," + std::to_string(i) + "\n";
			}
		}

		for (Uint_32 i = 1; i < camera_system.new_id; i++)
		{
			Uint_32 camera_entity = camera_system.get_entity(i);
			if (camera_entity)
			{
				entities_in_scene[camera_entity] += camera_system.get_component(i).to_string() + "," + std::to_string(i) + "\n";
			}
		}


		for (Uint_32 i = 1; i < animator_system.new_id; i++)
		{
			Uint_32 animator_entity = animator_system.get_entity(i);
			if (animator_entity)
			{
				entities_in_scene[animator_entity] += animator_system.get_component(i).to_string() + "," + std::to_string(i) + "\n";
			}
		}

		for (Uint_32 i = 1; i < graphics_system.new_id; i++)
		{
			Uint_32 graphics_entity = graphics_system.get_entity(i);
			if (graphics_entity)
			{
				auto ed = Graphic::editor_data[i];
				entities_in_scene[graphics_entity] += graphics_system.get_component(i).to_string() + "," + std::to_string(ed.future_tag)
					+ "," + std::to_string(ed.scale) + "\n";


				// save material
				Graphic& graphic = graphics_system.get_component(i);
				Material* mat = Material::get_material(graphic.element.material);
				std::string& mat_name = Material::material_names[graphic.element.material];
				std::string& shader_name = Shader::shader_names[mat->shader];
				std::string mat_line = "texture ";

				for (Uint_32 _tex_id = 0; _tex_id < 8; _tex_id++)
				{
					if (mat->diffuse_textures[_tex_id] != -1)
						mat_line += Texture::texture_names[mat->diffuse_textures[_tex_id]] + ",";
				}

				mat_line = mat_line.substr(0, mat_line.size() - 1);
				std::string shader_line = "shader " + shader_name;
				std::string material_definition = mat_line + "\n" + shader_line;
				write_to_file(material_definition, get_resource_path( mat_name), 1, 0);
			}
		}

		for (Uint_32 i = 0; i < PhysicsModule::collider_system.new_id; i++)
		{
			Uint_32 collider_entity = PhysicsModule::collider_system.get_entity(i);
			if (collider_entity)
				entities_in_scene[collider_entity] += PhysicsModule::collider_system.get_component(i).to_string() + "\n";
		}

		for (Uint_32 i = 0; i < PhysicsModule::rigidbody2d_system.new_id; i++)
		{
			Uint_32 rigidbody2d_entity = PhysicsModule::rigidbody2d_system.get_entity(i);
			if (rigidbody2d_entity)
				entities_in_scene[rigidbody2d_entity] += PhysicsModule::rigidbody2d_system.get_component(i).to_string() + "\n";
		}

		write_to_file("", scene_file, 0, 0); //clears file
		for (auto it : entities_in_scene)
		{
			write_to_file(it.second + "~\n", scene_file, 0, 1);
		}
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
