//
//  Display.cpp
//  Game Engine
//
//  Created by Aniruddha Prithul on 7/22/17.
//  Copyright © 2017 Aniruddha Prithul. All rights reserved.
//
#include "Display.hpp"

namespace Pringine {
    
    Display::Display(int width, int height, std::string title)
    {
        this->width = width;
        this->height = height;
        this->title = title;
        
        // initialize sdl and opengl related settings for graphics
        init();
        
        // create window
        window = SDL_CreateWindow(this->title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, this->width, this->height, SDL_WINDOW_OPENGL);
        
        // create the openGL context from the window  that was created
        glContext = SDL_GL_CreateContext(window);
        
        // if context creation failed, log the error
        if(glContext==NULL)
        {
            printf("SDL_Init failed: %s\n", SDL_GetError());
        }
        else
            printf("Context created with OpenGL version  (%s)\n", glGetString(GL_VERSION));


    }
    
    
    Display::~Display()
    {
        SDL_GL_DeleteContext(glContext);
        SDL_DestroyWindow(window);
        SDL_Quit();
    }
    
    void Display::init()
    {
        SDL_Init(SDL_INIT_EVERYTHING);
        
        SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
        SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
        SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
        SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
        SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE,32);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE,16);
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER,1);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3 );
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2 );
        
        glFrontFace(GL_CCW);     // points are gonna get supplid in clockwise order
        glDisable(GL_CULL_FACE);
        //glCullFace(GL_BACK);    // cull the back face
        //glEnable(GL_CULL_FACE); // enalbe face culling
        //glEnable(GL_DEPTH_TEST); //
        //glEnable(GL_FRAMEBUFFER_SRGB);  // auto gamma correction
        
    }
    
    void Display::Clear(float r, float g, float b, float a)
    {
        glClearColor(r, g, b, a);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
    }
    
    void Display::SwapBuffers()
    {
        SDL_GL_SwapWindow(window);
    }
    
    void Display::set_vsync(GLboolean value)
    {
        SDL_GL_SetSwapInterval(value);
    }
}
