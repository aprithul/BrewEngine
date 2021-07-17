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
//#include "GeometryLayer.hpp"
#include "SpriteLayer.hpp"
#include "ShapesLayer.hpp"
#include "GizmoLayer.hpp"
#ifdef EDITOR_MODE
#include "EditorLayer.hpp"
#endif
#include <vector>

namespace PrEngine {

	struct BatchPool
	{
		Uint_32 pool_size;
		BatchPool() = default;
		BatchPool(Uint_32 _pool_size);
		~BatchPool();
		Uint_32 get_new_batch();
		void return_batch(Uint_32 batch_id);
		void return_all_batches();
	private:
		Uint_32* pool;
		Bool_8* batch_in_use;


	};

    struct RendererOpenGL2D : public Module
    {
			std::vector<RenderLayer*> render_layers;
			std::vector<Graphic> static_batches;

			// display attributes
			Int_32 height;
			Int_32 width;

			// viewport attributes
			Vec2f viewport_pos;
			Vec2f viewport_size;

			std::string title;

			// constructor/ destructors
			RendererOpenGL2D(Int_32 width, Int_32 height, Bool_8 full_screen, std::string& title, std::string module_name,Int_32 priority);
			~RendererOpenGL2D();
			void update_viewport_size(int x, int y, int width, int height);
			void init();
			// clears the display with the color specified
			void Clear(Float_32 r, Float_32 g, Float_32 b, Float_32 a);
			// swaps buffers in a Double_64 buffer environment
			void SwapBuffers();
			void set_vsync(GLboolean value);
			Bool_8 make_shader_program(const std::string& path, GLuint& shader_program);
			GLuint make_shader( GLenum type, const std::string& source);
			//void generate_batched_sprite_graphics(Uint_32 graphic_id);
			void generate_sprite_graphics(Uint_32 graphic_id);
			void generate_sprite_graphics(Graphic& graphic, Uint_32 graphic_id = 0);
			void draw_line(Vec3f p1, Vec3f p2, Vec4f color);
			void draw_line_with_transform(Vec3f p1, Vec3f p2, Vec4f color, const Mat4x4& transformation);
			void draw_ray(Vec3f origin, Vec3f dir, Float_32 len, Vec4f color);
			void draw_rect(Rect<Float_32> rect, Vec4f color);
			void draw_rect_with_transform(Rect<Float_32> rect, Vec4f color, const Mat4x4& transformation);
			RenderLayer* get_layer(const std::string& layer_name);
			void prepare_batches(std::vector<Uint_32> batched_grphic_ids, Uint_32 usage);
			void render_graphic(const Graphic& graphic, Uint_32 elemtn_count, Mat4x4& transformation, Camera& _camera);

			void prepare_array_textures(std::vector<Uint_32>& texture_ids);
			BatchedGraphic* make_dynamic_batch(std::vector<Uint_32>& graphic_ids);
			void add_to_batch(std::vector< Uint_32>& graphic_ids);
			void add_to_static_batch(std::vector< Uint_32>& graphic_ids);
			void add_to_batch(Uint_32 graphic_id, RenderTag render_mode);
			//void prepare_dynmic_batches(std::vector<Uint_32> batched_grphic_ids);
			//Matrix4x4<Float_32> view_matrix;
			//Matrix4x4<Float_32> projection;

			void start() override;
			void update() override;
			void end() override;
			// reference to the sdl window
			SDL_Window* window;
			// the opengl context used with the window
			SDL_GLContext glContext;
			BatchPool* batch_pool;
			Int_32 is_included_at_index[99999];
			std::vector<Uint_32> array_textures;
			GLint max_layers;


			//static std::unordered_map<Uint_32, Uint_32> texture_to_index;
    };

	extern RendererOpenGL2D* renderer;


}

#endif /* Display_h */
