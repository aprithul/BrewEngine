//
//  Display.h
//  Game Engine
//
//  Created by Aniruddha Prithul on 7/22/17.
//  Copyright © 2017 Aniruddha Prithul. All rights reserved.
//

#ifndef RENDERER_OPENGL_2D_HPP
#define RENDERER_OPENGL_2D_HPP

#include "tiny_obj_loader.h"
#include "GlAssert.hpp"

#include "Material.hpp"
#include "Graphics.hpp"
#include "Module.hpp"
#include "RenderLayer.hpp"
#include "GeometryLayer.hpp"
#include "SpriteLayer.hpp"
#include "GuiLayer.hpp"
#include <vector>

namespace PrEngine {

    class RendererOpenGL2D : public Module
    {

		public:
			std::vector<RenderLayer*> render_layers;

			// display attributes
			Int_32 height;
			Int_32 width;
			std::string title;

			// constructor/ destructors
			RendererOpenGL2D(Int_32 width, Int_32 height, std::string title, std::string module_name,Int_32 priority);
			~RendererOpenGL2D();
			void init();
			// clears the display with the color specified
			void Clear(Float_32 r, Float_32 g, Float_32 b, Float_32 a);
			// swaps buffers in a Double_64 buffer environment
			void SwapBuffers();
			void set_vsync(GLboolean value);
			Bool_8 make_shader_program(const std::string& path, GLuint& shader_program);
			GLuint make_shader( GLenum type, const std::string& source);
			void generate_sprite_graphics(Uint_32 entity_id, const std::string& texture_file_path, const std::string& mat_name);
			RenderLayer* get_layer(const std::string& layer_name);
			//Matrix4x4<Float_32> view_matrix;
			//Matrix4x4<Float_32> projection;

			void start() override;
			void update() override;
			void end() override;
			// reference to the sdl window
			SDL_Window* window;
			// the opengl context used with the window
			SDL_GLContext glContext;

    };
}

#endif /* Display_h */
