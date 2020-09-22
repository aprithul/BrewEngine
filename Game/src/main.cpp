//  main.cpp
//  Game Engine
//
//  Created by Aniruddha Prithul on 7/22/17.
//  Copyright © 2017 Aniruddha Prithul. All rights reserved.
#include <stdlib.h>
#include <iostream>
#include "Module.hpp"
//#include <emscripten.h>
#if defined(EMSCRIPTEN) && defined(_WIN64)
#include "NetworkManager.hpp"
#endif
#include "Utils.hpp"
#include "Vector2.hpp"
#include "Engine.hpp"
#include "TimeModule.hpp"
#include "InputModule.hpp"
#include "EntityManagementSystemModule.hpp"
//#include "RendererModule.hpp"
#include "RendererOpenGL2D.hpp"
#include "FrameRateRegulatorModule.hpp"
#include "FrameRateCounterModule.hpp"
#include "Sprite.hpp"
#include "Camera3D.hpp"
#include "DirectionalLight.hpp"-
#include "Game.hpp"
#include "EntityGenerator.hpp"
//#include "ImGuiModule.hpp"

//using namespace PrEngine;


namespace PrEngine{

	//void main_loop(void* game_engine);
	bool is_running = true;

	int make_engine(){
		// helps us reload into editor or game mode
		bool restart = true;
		// create a new game engine instance
		while(restart)
		{
			restart = false;
			Engine* game_engine = new Engine();
			//restart = false;
			// add the components
			// render, frame regulator and frame counter should be the last three components updated ( and so added to engine )
			game_engine->add_module( new Time("Time", 0));
			game_engine->add_module(new InputManager("Input",1));
			game_engine->add_module(new EntityManagementSystem("EMS", 2));
			game_engine->add_module( new FrameRateRegulator("Frame Rate Regulator", 100000));
				((FrameRateRegulator*)game_engine->get_module("Frame Rate Regulator"))->set_uncapped();
			game_engine->add_module(new FrameRateCounter("Frame Counter", 100001));
			game_engine->add_module(new Game("game module",3));
			game_engine->add_module(new RendererOpenGL2D(1280,720,"PrEngine", "Renderer", 20));
				((RendererOpenGL2D*)game_engine->get_module("Renderer"))->set_vsync(true);


			#if defined(EMSCRIPTEN) && defined(_WIN64)
			NetworkManager* network_manager = (NetworkManager*) game_engine->
									add_module(new NetworkManager("Network manager", 5));
			#endif

			std::vector<std::string> file_paths;
			for(int i=0; i<27; i++)
			{
				file_paths.push_back("braid"+PATH_SEP+"tim_run"+PATH_SEP+std::to_string(i)+".gif");
			}

			EntityGenerator* entity_generator = new EntityGenerator();
			entity_generator->make_camera();
			entity_generator->make_animated_sprite_entity(file_paths,27);
			entity_generator->make_light_entity();

			game_engine->start();

			//show_file_structure( get_resource_path(""),"");

			#ifndef EMSCRIPTEN
					game_engine->update();
					game_engine->end();
					delete game_engine;
		}


		#else
				emscripten_set_main_loop_arg(main_loop, game_engine, 0, 1);
		#endif

		return 0;
	}


	/*void main_loop(void* game_engine)
	{
			((Engine*)game_engine)->update();
	}*/

}
int main(int argc, char * argv[])
{
	std::cout << "hello" << std::endl;
	PrEngine::make_engine();
	std::cout << "world" << std::endl;

	return 0;
}
