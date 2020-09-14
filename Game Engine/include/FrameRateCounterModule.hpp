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
        FrameRateCounter(std::string name, int priority);
        ~FrameRateCounter();
        //Renderer2D* renderer2D;

    private:
        void start() override;
        void update() override;
        void end() override;
        double one_second;
        int frame_count;
        int last_frame_count;
        //TTF_Font* font;
        //SDL_Texture* text_texture;
    };
}


#endif /* FrameRateCounter_hpp */
