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
		Timer(Double_64 duration, std::function<void()> callback, Bool_8 recurring );
		std::function<void()> callback;
		Double_64 target_duration;
		Double_64 current_duration;
		Bool_8 recurring;

	};

	class Time: public Module
    {
    public:
        Time(std::string name, Int_32 priority);
        ~Time();
        void update() override;
        void start() override;
        void end() override;
        static Double_64 get_time();
        static Double_64 get_frame_start_time();
        static Double_64 Frame_time;
        static Timer* make_timer(Double_64 duration, std::function<void()> callback, Bool_8 recurring);
        
    private:
        static Double_64 current_frame_start_time;
        static Double_64 last_frame_start_time;
        static std::vector<Timer> timers;
    };

}

#endif /* TimeComponent_hpp */
