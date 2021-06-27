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
#include <vector>
#include "Vec.hpp"
#include "Logger.hpp"
#include "Platform.hpp"

#if defined(_WIN64)
    const std::string PATH_SEP = "/";
	#include <windows.h>
#elif defined(_SWITCH)
    const std::string PATH_SEP = "/";
#elif defined(_WEB)
	const std::string PATH_SEP = "/";
#endif

namespace PrEngine
{
	extern std::string read_file(const std::string& file_name);
    extern void write_to_file(const std::string& text, const std::string& file_name, Bool_8 binary, Bool_8 append);
    extern std::string get_resource_path(const std::string &subDir = "");
    
	template <typename T>
	struct Rect
    {
        T x;
        T y;
        T w;
        T h;
    };
    


	Uint_32 str_hash(const std::string& str);

	std::vector<std::string> get_files_in_dir(const std::string& dir_name, const std::string& ext = "");
	void trim(std::string& str);
	
	/* includes range */
	Float_32 RandomFraction(Float_32 low, Float_32 high);
	//Bool_8 inside(Vec2f pos, SDL_FRect& rect, Bool_8 centered = false);

}
#endif
