//
//  Engine.hpp
//  Game Engine
//
//  Created by Aniruddha Prithul on 7/22/17.
//  Copyright © 2017 Aniruddha Prithul. All rights reserved.
//

#ifndef ENGINE_HPP
#define ENGINE_HPP

#include "Module.hpp"
#include "Utils.hpp"
#include "InputModule.hpp"
#include "EntityManagementSystemModule.hpp"
#include "Logger.hpp"
#include <map>
#include <vector>
#include <algorithm>


namespace PrEngine {
    // pair typedef, used as component map key
    // string is the name of the component
    // int is the order of execution
    //typedef std::pair<std::string, int> string_int_pair;
    
    // comparator object, used to sort map so that execution order is maintained
    /*struct CompareComponent
    {
        bool operator()(const string_int_pair& a, const string_int_pair& b) const {
            return a.second < b.second;
        }
    };*/
    // engine class to manage engine components
    class Engine
    {
    public:
        Engine();
        ~Engine();
        Module* add_module(Module* _module);
        Module* get_module(std::string);
        void start();
        void update();
        void end();
        void set_frame_rate(int frame_rate);
        bool compare(std::string a, std::string b);
    private:
        // map to store all components
        // sorted in execution order
        //std::map< string_int_pair, Component*, CompareComponent> engine_components;
        static bool priority_comparer(Module* a, Module* b);
        std::vector<Module*> engine_modules;
        int frame_rate;
        double frame_delta;
        bool is_running;

    };

    extern Engine* engine;

}

#endif /* Engine_hpp */
