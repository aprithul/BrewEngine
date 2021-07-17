#include "ImGuiConfiguration.hpp"


namespace PrEngine
{
	void initialize_imgui(SDL_Window* window, void* gl_context)
	{
		IMGUI_CHECKVERSION();
		if (ImGui::GetCurrentContext()) // only create new context if no previous contexts exist
			return;

		ImGui::CreateContext();
		ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
		ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		ImGuiIO& io = ImGui::GetIO(); (void)io;
#ifdef _WIN64
		io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
		io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;
#elif _SWITCH
		io.BackendFlags |= ImGuiBackendFlags_HasGamepad;
#endif
		// Setup Dear ImGui style
		ImGui::StyleColorsDark();
		ImGui_ImplOpenGL3_Init("#version 300 es");
		//inspector initialization
		//inspector_active = true;
		//ImGui::StyleColorsClassic();
	}
}