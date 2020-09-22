#ifndef UTILS_HPP
#define UTILS_HPP

#include <stdlib.h>
#include <string>
#include <stdio.h>
#include <ctype.h>
#include <SDL2/SDL.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include "Vector2.hpp"
#include "Logger.hpp"

#ifdef _WIN32
    const std::string PATH_SEP = "\\";
#else
    const std::string PATH_SEP = "/";
#endif

namespace PrEngine
{
	extern std::string read_file(const std::string& file_name);
    extern void write_to_file(const std::string& text, const std::string& file_name, bool binary);
    extern std::string get_resource_path(const std::string &subDir); 
    
    struct Rect
    {
        float x;
        float y;
        float w;
        float h;
        
        Rect(float x,float y,float w,float h);
        Rect();
    };
    

    template<typename T>
    T clamp(T value, T low, T high)
    {
        if(value > high)
            return high;
        if(value<low)
            return low;
        else
            return value;
    }

    template<typename T>
    int sign(T v)
    {
        if(v >= 0)
            return 1;
        else return -1;
    }

    template<typename T>
    T abs(T v)
    {
        if(v > 0)
            return v;
        else if (v < 0)
            return -v;
        else
            return 0;
    }

    bool inside(Vector2<int> pos, SDL_Rect& rect, bool centered = false);
    bool inside(Vector2<float> pos, SDL_Rect& rect, bool centered = false);
    //bool inside(Vector2<float> pos, SDL_FRect& rect, bool centered = false);

}
#endif
