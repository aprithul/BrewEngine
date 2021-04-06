#include "Platform.hpp"

#include <iostream>

//#define DEBUG false

#ifdef _WIN64
	#define Assert(x) if(!x) __debugbreak();
//#else
//	#define Assert(x) if(!x) __builtin_trap();
#endif

#if defined(DEBUG) && defined(_WIN64)
    #define GL_CALL(x) gl_clear_error(); x; Assert(gl_get_error(#x, __FILE__, __LINE__))
#else
    #define GL_CALL(x) x;
#endif

void gl_clear_error();
Bool_8 gl_get_error(Char_8* func_name, Char_8* file_name, Int_32 line_no);
