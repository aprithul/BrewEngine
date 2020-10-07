#include <GL/glew.h>

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#elif _WIN64
#include <GL/glew.h>
#endif

#include <iostream>
#include"Types.hpp"

#define DEBUG true

#ifdef _WIN64
	#define Assert(x) if(!x) __debugbreak();
#else
	#define Assert(x) if(!x) __builtin_trap();
#endif

#ifdef DEBUG
    #define GL_CALL(x) gl_clear_error(); x; Assert(gl_get_error(#x, __FILE__, __LINE__))
#else
    #define GL_CALL(x) x
#endif

void gl_clear_error();
Bool_8 gl_get_error(Char_8* func_name, Char_8* file_name, Int_32 line_no);
