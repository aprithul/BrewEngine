//
//  FrameRateRegulatorComponent.hpp
//  Game Engine
//
//  Created by Aniruddha Prithul on 7/28/17.
//  Copyright © 2017 Aniruddha Prithul. All rights reserved.
//

#ifndef FrameRateRegulatorComponent_hpp
#define FrameRateRegulatorComponent_hpp

#include "Module.hpp"
#include "TimeModule.hpp"
#include "SDL2/SDL.h"


namespace PrEngine {
    class FrameRateRegulator:public Module
    {
    public:
        FrameRateRegulator(std::string name, Int_32 priority);
        ~FrameRateRegulator();
        void start() override;
        void update() override;
        void end() override;
        void set_frame_rate(Int_32 frame_rate);
        void set_uncapped();
    private:
        Int_32 target_frame_rate;
        Double_64 frame_delta;
        Double_64 idle_time;
        Double_64 target_time;
        Double_64 one_second;
        Double_64 sleep_time;
        Int_32 frame_count;
        
    };
}

#endif /* FrameRateRegulatorComponent_hpp */
