//  main.cpp
//  Game Engine
//
//  Created by Aniruddha Prithul on 7/22/17.
//  Copyright © 2017 Aniruddha Prithul. All rights reserved.
#include "PlatformDefines.hpp"
#include <stdlib.h>
#include <iostream>
//#include "Module.hpp"
#include "EngineMain.hpp"
#include "EntityGenerator.hpp"
#include "Game.hpp"
#include "SceneManager.hpp"
#ifdef  _SWITCH
#include <switch.h>
#endif //  _SWITCH



//#include "ImGuiModule.hpp"

Int_32 make_engine_and_start_game();
void main_loop(void* game_engine);

Int_32 main(Int_32 argc, char* argv[])
{
#if defined(_SWITCH) && defined(DEBUG)
	socketInitializeDefault();              // Initialize sockets
	nxlinkStdio();                          // Redirect stdout and stderr over the network to nxlink
#endif // _SWITCH


	std::cout << "Starting engine" << std::endl;
	make_engine_and_start_game();
	std::cout << "Clolsed engine" << std::endl;

#if defined(_SWITCH) && defined(DEBUG)
	socketExit();                           // Cleanup
#endif
	return 0;
}


Int_32 make_engine_and_start_game(){

#ifndef EMSCRIPTEN
	// if restart is true, program will restart if quit
	bool restart = false;
	// create a new game engine instance
	do
	{
#endif
		PrEngine::Engine* game_engine = setup_engine_with_parameters(1280, 720,"PrEngine", false);
		game_engine->add_module(new Game("game module", 3)); //All 'gameplay' code managed by game module 
															// priority = 3 means that game module is added after Time, Input and EntityManagementSystem modules

		std::string file_path = std::string("braid") + PATH_SEP + "tim_run" + PATH_SEP + std::to_string(0) + ".gif";
		//entity_generator->make_camera_orthographic(16.f, 9.f);
		
		//for (int i = 0; i < MAX_GRAPHIC_COUNT / 10; i++)
		//{

		SceneManager::get_instance()->load_scene("scene.graph");
		/*auto p = entity_generator->make_animated_sprite_entity(file_path);
		auto c = entity_generator->make_animated_sprite_entity(file_path);
		entity_management_system->set_parent_transform(p, c);
		auto i = entity_generator->make_animated_sprite_entity(file_path);*/
		

		//EntityGenerator eg;
		//eg.make_camera_orthographic(16, 9, -10, 10);
		////eg.make_sprite("Materials" + PATH_SEP + "Tim.mat", Point3d{ 0,0,0 }, RENDER_STATIC);

		//Float_32 x_half_range = 1;
		//Float_32 y_half_range = 1;
		//int no_of_entities = 0;
		//for (Float_32 i = -x_half_range; i <= x_half_range; i++)
		//{
		//	for (Float_32 j = -y_half_range; j <= y_half_range; j++)
		//	{
		//		eg.make_sprite("Materials" + PATH_SEP + "Tim.mat", Point3d{i, j, 0 }, RENDER_STATIC);

		//		/*Uint_32 entity = eg.make_animated_sprite(Point3d{ i, j, 0 }, "Animations" + PATH_SEP + "run.anim", "Materials" + PATH_SEP + "Tim.mat");
		//		Uint_32 animator = entities[entity][COMP_ANIMATOR];
		//		animators[animator].animation_speed = RandomFraction(0.01f, 2.f);*/
		//		no_of_entities++;
		//	}
		//}
		//entity_management_system->update_transforms();
		//eg.generate_batches();
		////}
		//PrEngine::LOG(PrEngine::LOGTYPE_WARNING, std::to_string(PrEngine::entity_count));

		//std::string scene_name = "scene.graph";
		//entity_generator->load_scenegraph(scene_name);
		//entity_generator->make_light_entity();

		game_engine->start();
		//show_file_structure( get_resource_path(""),"");
	#ifndef EMSCRIPTEN
		game_engine->update();
		game_engine->end();
		delete game_engine;

	} while (restart);

	#else
			emscripten_set_main_loop_arg(main_loop, game_engine, 0, 1);
	#endif

	return 0;
}


void main_loop(void* game_engine)
{
		((Engine*)game_engine)->update();
}

