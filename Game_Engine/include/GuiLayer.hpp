#ifndef GUI_LAYER_HPP
#define GUI_LAYER_HPP

#include "Platform.hpp"
#include "RenderLayer.hpp"
#include "TimeModule.hpp"
#include "../Vendor/imgui-master/imgui.h"
#include "../Vendor/imgui-master/backends/imgui_impl_opengl3.h"
#include "../Vendor/imgui-master/backends/imgui_impl_sdl.h"
#include "Vec.hpp"
#include <string>
namespace PrEngine
{
    class GuiLayer : public RenderLayer
    {
        public:
			// needs to be assigned from game code

            GuiLayer(SDL_Window* sdl_window, SDL_GLContext* gl_context);
            ~GuiLayer();

            void start() override;
            void update() override;
            void end() override;

            Vec2f *panning;
            Vec2f *tiling;
			Int_32 fps;
        private:
            SDL_Window* window;
            SDL_GLContext* gl_context;
            
            // inspector
            Bool_8 inspector_active;
			void add_script(Uint_32 entity, std::string& script_name);
			void draw_scene_hierarchy();
			void draw_inspector_window();
			void draw_asset_window();
			void draw_editor(SDL_Window* window);
#ifdef DEBUG
			void draw_debug_window(Float_32 fps);
#endif // DEBUG
			//void draw_editor();


    };
}


#endif