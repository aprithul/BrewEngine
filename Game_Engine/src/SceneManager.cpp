#include "SceneManager.hpp"
#include "EntityManagementSystemModule.hpp"
#include "EntityGenerator.hpp"

namespace PrEngine {

	SceneManager* SceneManager::instance;

	void SceneManager::load_scene(const std::string & scene_name)
	{
		EntityGenerator eg;
		std::string scene_file_name_full = get_resource_path("Scenes" + PATH_SEP + scene_name);
		eg.load_scenegraph(scene_file_name_full);

		current_scene_name = scene_file_name_full;
	}

	bool SceneManager::save_scene(const std::string & scene_name)
	{
		std::string scene_file_name_full = get_resource_path("Scenes" + PATH_SEP + scene_name);
		entity_management_system->save_scene(scene_file_name_full);
		return true;
	}

	bool SceneManager::save_current_scene()
	{
		entity_management_system->save_scene(current_scene_name);
		return true;
	}

	SceneManager* SceneManager::get_instance()
	{
		if (instance == nullptr)
		{
			instance = new SceneManager();
		}
		return instance;
	}

	SceneManager::SceneManager()
	{

	}
}