//
//  TimeComponent.hpp
//  Game Engine
//
//  Created by Aniruddha Prithul on 7/26/17.
//  Copyright © 2017 Aniruddha Prithul. All rights reserved.
//

#ifndef TimeComponent_hpp
#define TimeComponent_hpp

#include "Module.hpp"
#include "SDL2/SDL.h"
#include "Logger.hpp"
#include <vector>
#include <functional>

namespace PrEngine {

	struct Timer
	{
		Timer(double duration, std::function<void()> callback, bool recurring );
		std::function<void()> callback;
		double target_duration;
		double current_duration;
		bool recurring;

	};

	class Time: public Module
    {
    public:
        Time(std::string name, int priority);
        ~Time();
        void update() override;
        void start() override;
        void end() override;
        static double get_time();
        static double get_frame_start_time();
        static double Frame_time;
        static Timer* make_timer(double duration, std::function<void()> callback, bool recurring);
        
    private:
        static double current_frame_start_time;
        static double last_frame_start_time;
        static std::vector<Timer> timers;
    };

}

#endif /* TimeComponent_hpp */
