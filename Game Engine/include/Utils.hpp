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
#include "Vector3.hpp"
#include "Logger.hpp"
#include "Types.hpp"

#ifdef _WIN32
    const std::string PATH_SEP = "\\";
#else
    const std::string PATH_SEP = "/";
#endif

namespace PrEngine
{
	extern std::string read_file(const std::string& file_name);
    extern void write_to_file(const std::string& text, const std::string& file_name, Bool_8 binary, Bool_8 append);
    extern std::string get_resource_path(const std::string &subDir); 
    
    struct Rect
    {
        Float_32 x;
        Float_32 y;
        Float_32 w;
        Float_32 h;
        
        Rect(Float_32 x,Float_32 y,Float_32 w,Float_32 h);
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
    Int_32 sign(T v)
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

    Bool_8 inside(Vector2<Int_32> pos, SDL_Rect& rect, Bool_8 centered = false);
	Bool_8 inside(Vector2<Float_32> pos, SDL_Rect& rect, Bool_8 centered = false);
	Bool_8 inside(Vector2<Float_32> pos, Rect& rect, Bool_8 centered = false);

	Uint_32 str_hash(const std::string& str);

	//Bool_8 inside(Vector2<Float_32> pos, SDL_FRect& rect, Bool_8 centered = false);

}
#endif
