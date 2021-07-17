#ifndef PLATFORM_DEFINES_HPP
#define PLATFORM_DEFINES_HPP

// first undef everything
#undef _SWITCH
#undef _WIN64
#undef _WEB
#undef EDITOR_MODE
#undef DEBUG

// enable the ones needed

//#define _SWITCH 1
#define _WIN64 1
//#define _WEB 1
#define EDITOR_MODE 1
//#define DEBUG 1

// if editor was enabled by mistake

//#ifndef _WIN64
//#undef EDITOR_MODE
//#endif // !_WIN64

#endif