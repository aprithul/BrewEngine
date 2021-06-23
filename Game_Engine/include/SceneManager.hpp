#ifndef SCENE_MANAGER_HPP
#define SCENE_MANAGER_HPP
#include <string>

namespace PrEngine {

	struct SceneManager
	{

	public:
		void load_scene(const std::string& scene_name);
		bool save_scene(const std::string& scene_name);
		bool save_current_scene();

		static SceneManager* get_instance();
		std::string current_scene_name;


	private:
		SceneManager();
		static SceneManager* instance;

		// delete any means of modifying this instance
		SceneManager& operator=(const SceneManager&) = delete;
		SceneManager(const SceneManager&) = delete;
		SceneManager& operator=(SceneManager&&) = delete;
		SceneManager(SceneManager&&) = delete;

	};
}


#endif