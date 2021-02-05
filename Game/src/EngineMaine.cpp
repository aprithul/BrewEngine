#include "EngineMain.hpp"
#include "FrameRateRegulatorModule.hpp"
#include "FrameRateCounterModule.hpp"
#include "RendererOpenGL2D.hpp"
#include "Game.hpp"
#if defined(EMSCRIPTEN) && defined(_WIN64)
#include "NetworkManager.hpp"
#endif

using namespace PrEngine;

Engine* setup_engine_with_parameters(Uint_32 window_width, Uint_32 window_height, std::string window_name, Bool_8 is_fullscreen )
{
	Engine* game_engine = new Engine();
	//restart = false;
	// add the components
	// render, frame regulator and frame counter should be the last three components updated ( and so added to engine )
	game_engine->add_module(new Time("Time", 0));
	game_engine->add_module(new InputManager("Input", 1));
	game_engine->add_module(new EntityManagementSystem("EMS", 2));
	game_engine->add_module(new FrameRateRegulator("Frame Rate Regulator", 100000));
	((FrameRateRegulator*)game_engine->get_module("Frame Rate Regulator"))->set_uncapped();
	((FrameRateRegulator*)game_engine->get_module("Frame Rate Regulator"));//->set_uncapped();
	//((FrameRateRegulator*)game_engine->get_module("Frame Rate Regulator"))->set_frame_rate(60);
	game_engine->add_module(new FrameRateCounter("Frame Counter", 100001));
	game_engine->add_module(new RendererOpenGL2D(window_width, window_height, is_fullscreen, window_name, "Renderer", 99999));
	((RendererOpenGL2D*)game_engine->get_module("Renderer"))->set_vsync(true);


#if defined(EMSCRIPTEN) && defined(_WIN64)
	NetworkManager* network_manager = (NetworkManager*)game_engine->
		add_module(new NetworkManager("Network manager", 5));
#endif

	return game_engine;
}
