//  main.cpp
//  Game Engine
//
//  Created by Aniruddha Prithul on 7/22/17.
//  Copyright © 2017 Aniruddha Prithul. All rights reserved.
#include <stdlib.h>
#include <iostream>
//#include "Module.hpp"
#include "EngineMain.hpp"
#include "EntityGenerator.hpp"
#include "Game.hpp"
//#include <emscripten.h>

//#include "ImGuiModule.hpp"

Int_32 make_engine_and_start_game();

Int_32 main(Int_32 argc, char* argv[])
{
	make_engine_and_start_game();

	return 0;
}


Int_32 make_engine_and_start_game(){

	// if restart is true, program will restart if quit
	bool restart = false;
	// create a new game engine instance
	do
	{
		PrEngine::Engine* game_engine = setup_engine_with_parameters(1280, 720,"PrEngine Demo", false);
		game_engine->add_module(new Game("game module", 3)); //All 'gameplay' code managed by game module 
															// priority = 3 means that game module is added after Time, Input and EntityManagementSystem modules

		std::string file_path = std::string("braid") + PATH_SEP + "tim_run" + PATH_SEP + std::to_string(0) + ".gif";
		PrEngine::EntityGenerator* entity_generator = new PrEngine::EntityGenerator();
		//entity_generator->make_camera_orthographic(16.f, 9.f);
		
		//for (int i = 0; i < MAX_GRAPHIC_COUNT / 10; i++)
		//{
		/*auto p = entity_generator->make_animated_sprite_entity(file_path);
		auto c = entity_generator->make_animated_sprite_entity(file_path);
		entity_management_system->set_parent_transform(p, c);
		auto i = entity_generator->make_animated_sprite_entity(file_path);*/


		//}
		PrEngine::LOG(PrEngine::LOGTYPE_WARNING, std::to_string(PrEngine::entity_management_system->entity_count));

		//std::string scene_name = "scene.graph";
		//entity_generator->load_scenegraph(scene_name);
		//entity_generator->make_light_entity();

		game_engine->start();
		entity_generator->load_scenegraph("scene.graph");

		//show_file_structure( get_resource_path(""),"");
	#ifndef EMSCRIPTEN
		game_engine->update();
		game_engine->end();
		delete game_engine;
	}
	while (restart);


	#else
			emscripten_set_main_loop_arg(main_loop, game_engine, 0, 1);
	#endif

	return 0;
}


/*void main_loop(void* game_engine)
{
		((Engine*)game_engine)->update();
}*/

