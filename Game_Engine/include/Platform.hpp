#ifndef PLATFORM_HPP
#define PLATFORM_HPP
#include "PlatformDefines.hpp"
#include "Types.hpp"


#ifdef _WIN64
#include <GL/glew.h>
#elif _SWITCH
#include <GLES2/gl2.h>  // OpenGL ES 2.0 library
#elif __APPLE__
#include <OpenGL/gl3.h>
#endif

#endif
