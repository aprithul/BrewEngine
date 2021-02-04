#ifndef GUI_LAYER_HPP
#define GUI_LAYER_HPP

#include "RenderLayer.hpp"
#include "TimeModule.hpp"
#include "../Vendor/imgui-master/imgui.h"
#include "../Vendor/imgui-master/imgui_impl_opengl3.h"
#include "../Vendor/imgui-master/imgui_impl_sdl.h"
#include "Vector2.hpp"
#include <string>
namespace PrEngine
{
    class GuiLayer : public RenderLayer
    {
        public:
            GuiLayer(SDL_Window* sdl_window, SDL_GLContext* gl_context);
            ~GuiLayer();

            void start() override;
            void update() override;
            void end() override;
            Vector2<Float_32> *panning;
            Vector2<Float_32> *tiling;
			Int_32 fps;
        private:
            SDL_Window* window;
            SDL_GLContext* gl_context;
            
            // inspector
            Bool_8 inspector_active;
			void draw_editor();


    };
}


#endif