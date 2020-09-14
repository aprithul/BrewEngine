//
//  Display.h
//  Game Engine
//
//  Created by Aniruddha Prithul on 7/22/17.
//  Copyright © 2017 Aniruddha Prithul. All rights reserved.
//

#ifndef Display_h
#define Display_h

#include <iostream>
#include <SDL2/SDL.h>
#include <OpenGL/gl3.h>

namespace Pringine {
    
    class Display
    {
    public:
        // display attributes
        int height;
        int width;
        std::string title;
        
        // constructor/ destructors
        Display(int width, int height, std::string title);
        ~Display();
        void init();
        // clears the display with the color specified
        void Clear(float r, float g, float b, float a);
        // swaps buffers in a double buffer environment
        void SwapBuffers();
        void set_vsync(GLboolean value);
        
    private:
        // reference to the sdl window
        SDL_Window* window;
        // the opengl context used with the window
        SDL_GLContext glContext;
        
        
    };
}


#endif /* Display_h */
