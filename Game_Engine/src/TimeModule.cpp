//
//  TimeComponent.cpp
//  Game Engine
//
//  Created by Aniruddha Prithul on 7/26/17.
//  Copyright © 2017 Aniruddha Prithul. All rights reserved.
//


#include "TimeModule.hpp"

namespace PrEngine {
    
    Double_64 Time::Frame_time;
    Double_64 Time::current_frame_start_time;
    Double_64 Time::last_frame_start_time;
    std::vector<Timer> Time::timers;
    
    Time::Time(std::string name, Int_32 priority):Module(name,priority)
    {
    }
    
    Time::~Time()
    {
        timers.clear();
    }
    
    void Time::start()
    {
        
        Frame_time = 0;
        last_frame_start_time = 0;
        current_frame_start_time = 0;
    }
    
    void Time::update()
    {
        if(is_active)
        {

            last_frame_start_time = current_frame_start_time;
            current_frame_start_time = SDL_GetTicks()/1000.0;
            Frame_time = current_frame_start_time - last_frame_start_time;

            // call callback, reset if recurring timer
            // otherwise delete
        	for(std::vector<Timer>::iterator it = timers.begin(); it != timers.end();)
        	{
        		it->current_duration += Frame_time;
        		if(it->current_duration >= it->target_duration)
        		{
        			it->callback();

            		if(it->recurring)
            		{
            			it->current_duration = 0;
            			it++;
            		}
            		else
            		{
            			it = timers.erase(it);
            		}
        		}
        		else
        			it++;

        	}
        }
        
    }
    
    Double_64 Time::get_frame_start_time()
    {
        return current_frame_start_time;
    }
    
    Double_64 Time::get_time()
    {
        return  SDL_GetTicks()/1000.0;
    }
    
    
    void Time::end()
    {
        
    }
    
    Timer::Timer(Double_64 duration, std::function<void()> callback, Bool_8 recurring)
    {
    	this->current_duration = 0;
    	this->target_duration = duration;
    	this->callback = callback;
    	this->recurring = recurring;
    }

    Timer* Time::make_timer(Double_64 duration, std::function<void()> callback, Bool_8 recurring)
    {
    	Timer t(duration, callback,recurring);
    	timers.push_back(t);
    	return &(timers.back());

    }

}
