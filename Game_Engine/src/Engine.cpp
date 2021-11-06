//
//  Engine.cpp
//  Game Engine
//
//  Created by Aniruddha Prithul on 7/22/17.
//  Copyright © 2017 Aniruddha Prithul. All rights reserved.
//

#define PRI_VEC_IMPLEMENTATION

#include "Engine.hpp"
#include <time.h>
#include "Utils.hpp"
#include "PhysicsModule.hpp"
namespace PrEngine {
    
    Engine* Engine::engine;

    Engine::Engine()
    {
        engine = this;
        frame_rate = 0;
        frame_delta = 0;
        is_running = true;
    }
    
    Engine::~Engine()
    {
        
        for(Int_32 _i=0; _i<engine_modules.size(); _i++)
        {
            delete engine_modules[_i];
        }
        // can safley clear now
        engine_modules.clear();
        SDL_Quit();
        LOG(LOGTYPE_GENERAL, "Engine destroyed");
    }
    
    static Bool_8 priority_comparer(Module* a, Module* b)
    {
        return (a->priority<b->priority);        
    }

    // add a new component to the engine
    Module* Engine::add_module(Module* component)
    {
        engine_modules.push_back(component);
        return component;
    }
    
    // get the specified component
    Module* Engine::get_module(std::string component_name)
    {
        for(Int_32 _i=0; _i<engine_modules.size(); _i++)
        {
            if(engine_modules[_i]->name == component_name)
            {
                return engine_modules[_i];
            }
        }
        
        return NULL;
    }
    
    // called in the start
    void Engine::start()
    {
        this->is_running = true;
        for(Int_32 _i=0; _i<engine_modules.size(); _i++)
        {
                engine_modules[_i]->start();
        }

        // get the reference to the input module
        //input_manager = (InputManager*)this->get_module("Input");
    }
    
	Float_32 accum = 0;
    // called every frame
    void Engine::update()
    {
		//static std::string _text;
		//_text.reserve(324000);

		//static int _its = 5;
        // engine loop
        // updates at specified frame rate

        #ifndef EMSCRIPTEN
        while(this->is_running)
        #endif
        {
			//_its--;

            //LOG(LOGTYPE_GENERAL, "Updating: ");
            // update all components
			//clock_t begin = clock();

            for(Int_32 _i=0; _i<engine_modules.size(); _i++)
            {
				clock_t begin = clock();
                engine_modules[_i]->update();
				clock_t end = clock();
				Double_64 elapsed = (Double_64)(end - begin) / CLOCKS_PER_SEC;
				continue;
            }

			if (input_manager->was_crossed || input_manager->get_gamecontroller(0)->any_button_state)
				is_running = false;

			//clock_t end = clock();
			//Float_32 elap = ((Float_32)(end - begin) / CLOCKS_PER_SEC);
			//accum += elap;
			//_text += std::to_string(elap) + "\n";
			//Double_64 elapsed = (Double_64)(end - begin) / CLOCKS_PER_SEC;
			//if (elapsed > 0.02 && _its < 0)
			//{
			//	LOG(LOGTYPE_ERROR, "Exceeded frame time:" + std::to_string(elapsed));
			//	//assert(0);
			//}
            // check if window was crossed
			//if (accum >= 600)
			//	is_running = false;

            
            //if(input_handler->get_key_down(SDLK_q))
            //{
            //    is_running = false;
            //    LOG(LOGTYPE_GENERAL, "Key 'q' pressed");
            //}
        }
		
		

		//write_to_file(_text, get_resource_path("log.txt"), false, false);
    }
    
    // called in the end
    void Engine::end()
    {
        for(Int_32 _i=0; _i<engine_modules.size(); _i++)
        {
            engine_modules[_i]->end();
        }
    }
}
