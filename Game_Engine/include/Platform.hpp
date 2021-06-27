#ifndef PLATFORM_HPP
#define PLATFORM_HPP
#include "PlatformDefines.hpp"
#include "Types.hpp"


#ifdef _WIN64
#include <GL/glew.h>
#elif _SWITCH
#include <GLES2/gl2.h>  // OpenGL ES 2.0 library
#include <glad/glad.h>  // glad library (OpenGL loader)
#elif _WEB
	#include <emscripten.h>
	#include <GLES3/gl3.h>  // OpenGL ES 2.0 library
//#include <GL/glew.h>
//#include <SDL_opengles2.h>

#elif __APPLE__
#include <OpenGL/gl3.h>
#endif

#endif
