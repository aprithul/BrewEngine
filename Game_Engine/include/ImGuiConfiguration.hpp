#ifndef IMGUI_CONFIGURATION_HPP
#define IMGUI_CONFIGURATION_HPP

#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_sdl.h"

namespace PrEngine {
	void initialize_imgui(SDL_Window* window, void* gl_context);
}
#endif // !IMGUI_CONFIGURATION_HPP
