//
//  Engine.cpp
//  Game Engine
//
//  Created by Aniruddha Prithul on 7/22/17.
//  Copyright © 2017 Aniruddha Prithul. All rights reserved.
//

#include "Engine.hpp"


namespace PrEngine {
    
    Engine* Engine::engine;

    Engine::Engine()
    {
        engine = this;
        frame_rate = 0;
        frame_delta = 0;
        is_running = false;
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
    
    // called every frame
    void Engine::update()
    {
                
        // engine loop
        // updates at specified frame rate

        #ifndef EMSCRIPTEN
        while(this->is_running)
        #endif
        {
            //LOG(LOGTYPE_GENERAL, "Updating: ");
            // update all components
            for(Int_32 _i=0; _i<engine_modules.size(); _i++)
            {
                engine_modules[_i]->update();
            }
            
            // check if window was crossed
            if(input_manager->was_crossed)
                is_running = false;
            
            //if(input_handler->get_key_down(SDLK_q))
            //{
            //    is_running = false;
            //    LOG(LOGTYPE_GENERAL, "Key 'q' pressed");
            //}
        }
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
