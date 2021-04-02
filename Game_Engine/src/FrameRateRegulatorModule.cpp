//
//  FrameRateRegulatorComponent.cpp
//  Game Engine
//
//  Created by Aniruddha Prithul on 7/28/17.
//  Copyright © 2017 Aniruddha Prithul. All rights reserved.
//

#include "FrameRateRegulatorModule.hpp"

namespace PrEngine {
    FrameRateRegulator::FrameRateRegulator(std::string name, Int_32 priority):Module(name,priority)
    {
    }
    
    FrameRateRegulator::~FrameRateRegulator()
    {
        
    }
    
    void FrameRateRegulator::start()
    {
        idle_time   = 0;
        target_time = 0;
    }
    
    void FrameRateRegulator::update()
    {
        if(is_active)
        {
            idle_time +=this->frame_delta - (Time::get_time() - Time::get_frame_start_time());
            target_time = Time::get_time() + idle_time;
            //idle_time +=  this->frame_delta - Time::Frame_time;
            sleep_time = (Int_32)(idle_time*1000*0.8);
            if(sleep_time>0)
                SDL_Delay(sleep_time);
            
            while( Time::get_time() < target_time)
            {
                continue;
            }
            idle_time = target_time - Time::get_time();
            
        }
    }
    
    void FrameRateRegulator::end()
    {
        
    }
    
    void FrameRateRegulator::set_frame_rate(Int_32 frame_rate)
    {
        this->target_frame_rate = frame_rate;
        this->frame_delta = 1.0/frame_rate;
    }

    void FrameRateRegulator::set_uncapped()
    {
        this->frame_delta = 0.0f;
    }

}
