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
			int height;
			int width;
			std::string title;

			// constructor/ destructors
			RendererOpenGL2D(int width, int height, std::string title, std::string module_name,int priority);
			~RendererOpenGL2D();
			void init();
			// clears the display with the color specified
			void Clear(float r, float g, float b, float a);
			// swaps buffers in a double buffer environment
			void SwapBuffers();
			void set_vsync(GLboolean value);
			bool make_shader_program(const std::string& path, GLuint& shader_program);
			GLuint make_shader( GLenum type, const std::string& source);
			Graphics* generate_sprite_graphics(const std::vector<std::string>& texture_file_paths, const std::string& mat_name);
			RenderLayer* get_layer(const std::string& layer_name);
			//Matrix4x4<float> view_matrix;
			//Matrix4x4<float> projection;

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
