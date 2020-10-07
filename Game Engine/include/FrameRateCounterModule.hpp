//
//  FrameRateCounter.hpp
//  Game Engine
//
//  Created by Aniruddha Prithul on 7/28/17.
//  Copyright © 2017 Aniruddha Prithul. All rights reserved.
//

#ifndef FrameRateCounter_hpp
#define FrameRateCounter_hpp

#include <string>
//#include <SDL2/SDL_ttf.h>
#include "Module.hpp"
#include "TimeModule.hpp"
#include "Logger.hpp"
//#include "RendererModule.hpp"
//#include "GUI.hpp"

namespace PrEngine {
    class FrameRateCounter:public Module
    {
    public:
        FrameRateCounter(std::string name, Int_32 priority);
        ~FrameRateCounter();
        //Renderer2D* renderer2D;

    private:
        void start() override;
        void update() override;
        void end() override;
        Double_64 one_second;
        Int_32 frame_count;
        Int_32 last_frame_count;
        //TTF_Font* font;
        //SDL_Texture* text_texture;
    };
}


#endif /* FrameRateCounter_hpp */
