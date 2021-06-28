//
//  Display.cpp
//  Game Engine
//
//  Created by Aniruddha Prithul on 7/22/17.
//  Copyright © 2017 Aniruddha Prithul. All rights reserved.
//
#include <SDL2/SDL.h>
#include "GlAssert.hpp"
#include "RendererOpenGL2D.hpp"
#include "Math.hpp"
#include <cmath>
#include <set>
#include <time.h>
#include <algorithm>

namespace PrEngine {

	RendererOpenGL2D* renderer = nullptr;
	GLint max_layers;
	Uint_32 max_vertices_in_batch;
	GLint max_texture_units = 0;

	RendererOpenGL2D::RendererOpenGL2D(Int_32 width, Int_32 height, Bool_8 full_screen, std::string& title, std::string module_name, Int_32 priority):Module(module_name, priority)
    {

		assert(renderer == nullptr);

		this->width = width;
		this->height = height;
		this->title = title;
		this->viewport_pos.x = 0;
		this->viewport_pos.y = 0;
		this->viewport_size.x = width;
		this->viewport_size.y = height;
		init();
/*
		SDL_Init(SDL_INIT_VIDEO);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
*/
		window = SDL_CreateWindow(this->title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_OPENGL);// | (full_screen*SDL_WINDOW_FULLSCREEN) );
		//window = SDL_CreateWindow(this->title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);// | (full_screen*SDL_WINDOW_FULLSCREEN) );
		glContext = SDL_GL_CreateContext(window);



#if defined(_WIN64)
		glewExperimental = GL_TRUE;
		if (glewInit() != GLEW_OK)
			printf("Glew not initialized properly");
#endif

#if defined(_WIN64)
		// GL_CALL(glFrontFace(GL_CCW))     // points are gonna get supplid in clockwise order
		// GL_CALL(glEnable(GL_CULL_FACE))
		GL_CALL(glDisable(GL_CULL_FACE));
		//GL_CALL(glEnable(GL_CULL_FACE)); // enalbe face culling
		GL_CALL(glCullFace(GL_BACK));    // cull the back face
		GL_CALL(glFrontFace(GL_CCW));
		//GL_CALL(glEnable(GL_BLEND))
		//GL_CALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA))
		GL_CALL(glEnable(GL_DEPTH_TEST))
			GL_CALL(glDepthFunc(GL_LESS))
			//GL_CALL(glEnable(GL_MULTISAMPLE))

			
#elif _SWITCH
		if (!gladLoadGL())
			LOG(LOGTYPE_ERROR, "Glad not initialized properly");
		else
			LOG(LOGTYPE_WARNING, "Successfully initialized GLAD");

		//GL_CALL(glEnable(GL_DEPTH_TEST))
		//GL_CALL(glDepthFunc(GL_LESS));

		//GL_CALL(glEnable(GL_BLEND))
		//GL_CALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA))

#endif

		if (glContext == NULL)
		{
			printf("SDL_Init failed: %s\n", SDL_GetError());
		}
		else
			printf("Context created with OpenGL version  (%s)\n", glGetString(GL_VERSION));


		GLuint id;

		const GLubyte* glVersion = glGetString(GL_VERSION);
		printf("GLVersion = %s\n", glVersion);

		const GLubyte* vendor = glGetString(GL_VENDOR);
		const GLubyte* renderer_v = glGetString(GL_RENDERER);
		printf("vendor = %s\nrenderer = %s\n", vendor, renderer_v);

		GLuint q;
		glGenQueries(1, &q);
		printf("If you see this there was no crash!\n");

		//glGenBuffers(1, &id);
		//GL_CALL(glGenBuffers(1, &id))
		//printf("Buffer: %d\n", id);

		printf("---------------------------------------------------\n");



		/*this->viewport_x = width * 0.7 / 6;
		this->viewport_y = height / 4;
		this->viewport_width = width * 4.3 / 6;
		this->viewport_height = height * 3 / 4;*/
        //light_direction =
        // initialize sdl and opengl related settings for graphics

        // create window
        // create the openGL context from the window  that was created
        //glContext = SDL_GL_CreateContext(window);

        std::cout<<"Loading default texture"<<std::endl;
        //Texture::load_default_texture();
		Uint_32 mat = Material::load_material("Materials/Default.mat", true);
		
		assert(Material::material_creation_status); // default material has to be created for engine to work

        //data = stbi_load( get_resource_path("default.jpg").c_str() ,&width, &height, &no_of_channels, 0);


        /// create render layers
        //GeometryLayer* geometry_layer = new GeometryLayer(camera_handle);
        //render_layers.push_back(geometry_layer);

        SpriteLayer* sprite_layer = new SpriteLayer();
        render_layers.push_back(sprite_layer);

		ShapesLayer* shapes_layer = new ShapesLayer();
		render_layers.push_back(shapes_layer);

		//GizmoLayer* gizmo_layer = new GizmoLayer();
		//render_layers.push_back(gizmo_layer);

		GuiLayer* gui_layer = new GuiLayer(window, &glContext);
		render_layers.push_back(gui_layer);

		renderer = this;


		GL_CALL(glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &max_texture_units))
		GL_CALL(glGetIntegerv(GL_MAX_ARRAY_TEXTURE_LAYERS, &max_layers))	//max depth of array textures

		LOG(LOGTYPE_WARNING, "vao res: " + std::to_string(max_texture_units));
		LOG(LOGTYPE_WARNING, "vao res: " + std::to_string(max_layers));

    }


	RendererOpenGL2D::~RendererOpenGL2D()
    {
        for(std::vector<RenderLayer*>::iterator it = render_layers.begin(); it != render_layers.end(); it++)
            delete (*it);

        Texture::delete_all_texture_data();
        Texture::delete_all_textures();
        Shader::delete_all_shaders();
        Material::delete_all_materials();

        SDL_GL_DeleteContext(glContext);
        SDL_DestroyWindow(window);
        SDL_Quit();

    }

	void RendererOpenGL2D::update_viewport_size(int x, int y, int width, int height)
	{
		viewport_pos.x = x;
		viewport_pos.y = y;
		viewport_size.x = width;
		viewport_size.y = height;

		glViewport(x, y, width, height);
	}

    void RendererOpenGL2D::init()
    {
        SDL_Init(SDL_INIT_VIDEO);
        //TTF_Init();

#if defined(_SWITCH) || defined(_WEB)
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
		//SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);

		/* Turn on double buffering with a 24bit Z buffer.
		 * You may need to change this to 16 or 32 for your system */
		//SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		//SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

#elif _WIN64

        SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
        SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
        SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
        SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
        SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE,32);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE,16);
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER,1);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1 );
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS,1);
        SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);
#endif
        //glEnable(GL_DEPTH_TEST);
        //glEnable(GL_FRAMEBUFFER_SRGB);  // auto gamma correction

    }

    void RendererOpenGL2D::Clear(Float_32 r, Float_32 g, Float_32 b, Float_32 a)
    {
        glClearColor(r, g, b, a);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    }

    void RendererOpenGL2D::SwapBuffers()
    {
        SDL_GL_SwapWindow(window);
    }

    void RendererOpenGL2D::set_vsync(GLboolean value)
    {
        SDL_GL_SetSwapInterval(value);
    }
    void RendererOpenGL2D::start()
    {
        for(std::vector<RenderLayer*>::iterator it=render_layers.begin(); it!=render_layers.end(); it++)
            (*it)->start();
    }

    void RendererOpenGL2D::update()
    {
		SDL_GetWindowSize(window, &width, &height);
        Clear(0,0,0.3f,1);
		clock_t begin = clock();

		for (std::vector<RenderLayer*>::iterator layer = render_layers.begin(); layer != render_layers.end(); layer++)
		{
			(*layer)->update();
		}
        SwapBuffers();
		clock_t end = clock();
		Double_64 elapsed = (Double_64)(end - begin) / CLOCKS_PER_SEC;

    }

    void RendererOpenGL2D::end()
    {
        for(std::vector<RenderLayer*>::iterator it=render_layers.begin(); it!=render_layers.end(); it++)
            (*it)->end();
    }

	std::vector<GLuint> indices;
	std::vector<Vertex> buffer;
	std::vector<Uint_32> batch_texture_ids;
	std::set<Uint_32> batch_textures_set;
	std::vector<Uint_32> batch_graphic_ids;
	Int_32 batch_count = 0;
	Uint_32 index = 0;

	inline void make_batch(std::vector<Uint_32>& texture_ids, Uint_32 usage)
	{
		VertexLayout layout;
		VertexAttribute attribute_0(0, 3, GL_FLOAT, GL_FALSE);
		VertexAttribute attribute_1(1, 4, GL_FLOAT, GL_FALSE);
		//VertexAttribute attribute_2(2, 3, GL_FLOAT, GL_FALSE);
		VertexAttribute attribute_3(2, 2, GL_FLOAT, GL_FALSE);
		VertexAttribute attribute_4(3, 1, GL_FLOAT, GL_FALSE);

		layout.add_attribute(attribute_0);
		layout.add_attribute(attribute_1);
		//layout.add_attribute(attribute_2);
		layout.add_attribute(attribute_3);
		layout.add_attribute(attribute_4);

		//batched_graphics.emplace_back();
		//auto& batch = batched_graphics.back();
		BatchedGraphic& batch = batched_graphics[next_batched_graphic_pos];
		LOG(LOGTYPE_WARNING, "load batch material");
		batch.element.material = Material::load_material("Materials" + PATH_SEP + "Batch.mat", false, std::to_string(batch_count++));
		next_batched_graphic_pos++;
		batch.transform_id = 0;	// use identity unless modified later ( eg. animations)

		if (usage == GL_STATIC_DRAW)
			batch.tag = RENDER_STATIC;
		else if (usage == GL_STREAM_DRAW)
		{
			batch.tag = RENDER_DYNAMIC;
			batch.graphic_ids = batch_graphic_ids;
			// store texture indices for dynamic vbo 
			for (int _i = 0; _i < texture_ids.size(); _i++)
			{
				batch.texture_to_index[texture_ids[_i]] = _i;
			}
			
		}
		
		Material* mat = Material::get_material(batch.element.material);

		//memcpy(mat->diffuse_textures, &texture_ids[0], sizeof(Uint_32) * texture_ids.size());

		// make array texture
		mat->diffuse_textures[0] = Texture::make_array_texture(texture_ids);

		batch.element.vao.Generate();
		LOG(LOGTYPE_WARNING, "vao generated: ",std::to_string(batch.element.vao.id));
		if (usage == GL_STATIC_DRAW)
		{
			batch.element.vbo.Generate(&buffer[0], buffer.size() * sizeof(Vertex), GL_STATIC_DRAW);
		}
		else
		{
			batch.element.vbo.Generate(nullptr, sizeof(Vertex) * BatchedGraphic::max_vertices_in_batch, GL_STREAM_DRAW);
		}
		LOG(LOGTYPE_WARNING, "vbo generated");

		batch.element.layout = layout;
		LOG(LOGTYPE_WARNING, "layout generated");

		for (Uint_32 _i=0; _i < batch.element.layout.next_attrib; _i++)// != batch.element.layout.vertex_attributes.end(); attr++)
		{
			VertexAttribute* attr = &(batch.element.layout.vertex_attributes[_i]);
			GL_CALL(
				glEnableVertexAttribArray(attr->index))
				//GL_CALL(
				glVertexAttribPointer(attr->index, attr->count, attr->type, attr->normalized, layout.stride, (void*)attr->offset);//)
		}

		batch.element.ibo.Generate(&indices[0], indices.size() * sizeof(GLuint), indices.size());
		batch.element.num_of_triangles = (buffer.size() / 3);
		LOG(LOGTYPE_WARNING, "ibo generated");

		batch.element.vao.Unbind();
		batch.element.ibo.Unbind();
		

		buffer.clear();
		indices.clear();
		batch_texture_ids.clear();
		batch_textures_set.clear();
		batch_graphic_ids.clear();
		index = 0;

		LOG(LOGTYPE_GENERAL, "Batch made: ", std::to_string(usage));

	}


	void process_vertecies(Uint_32 graphic_id, Uint_32 usage)
	{

		indices.push_back(index);
		indices.push_back(index + 1);
		indices.push_back(index + 2);
		indices.push_back(index + 2);
		indices.push_back(index + 3);
		indices.push_back(index);
		index += 4;


		Graphic& graphic = graphics[graphic_id];
		Uint_32 material_id = graphic.element.material;
		assert(material_id);
		Material* mat = Material::get_material(material_id);
		Texture* tex = Texture::get_texture(mat->diffuse_textures[0]); //Material::material_library[mat_id].diffuse_texture;
		Float_32 x_scale = tex->width;
		Float_32 y_scale = tex->height;
		Float_32 texco_u = clamp(tex->width / (Float_32)MAX_TEXTURE_SIZE, 0.1f, 1.0f);
		Float_32 texco_v = clamp(tex->height / (Float_32)MAX_TEXTURE_SIZE, 0.1f, 1.0f);

		if (x_scale > y_scale)
		{
			x_scale = (x_scale / y_scale);
			y_scale = 1.f;
		}
		else
		{
			y_scale = y_scale / x_scale;
			x_scale = 1.f;
		}
		GraphicEditorData ged = Graphic::editor_data[graphic_id];
		x_scale *= ged.scale;
		y_scale *= ged.scale;

		Transform3D& transform = transforms[graphic.transform_id];
		Vec3f p1 =  Vec3f{ 0.5f*x_scale, 0.5f*y_scale, 0.0f };
		Vec3f p2 =  Vec3f{ -0.5f*x_scale, 0.5f*y_scale, 0.0f };
		Vec3f p3 =  Vec3f{ -0.5f*x_scale, -0.5f*y_scale, 0.0f };
		Vec3f p4 =  Vec3f{ 0.5f*x_scale, -0.5f*y_scale, 0.0f };

		// save vertex data for collider
		Graphic::vertex_data[graphic_id][0] = p1;
		Graphic::vertex_data[graphic_id][1] = p2;
		Graphic::vertex_data[graphic_id][2] = p3;
		Graphic::vertex_data[graphic_id][3] = p4;

		if (usage == GL_STATIC_DRAW)
		{
			p1 = transform.transformation * p1;
			p2 = transform.transformation * p2;
			p3 = transform.transformation * p3;
			p4 = transform.transformation * p4;

			Uint_32 texture_id = mat->diffuse_textures[0];
			if (batch_texture_ids.size() == 0 || batch_texture_ids.back() != texture_id)
			{
				batch_texture_ids.push_back(texture_id);
			}
			Float_32 texture_index = batch_texture_ids.size() - 1;
			// find index of texture in set

			/*auto it = std::find(texture_ids.begin(), texture_ids.end(), texture_id);
			if (it != texture_ids.end())
			{
				texture_index = it - texture_ids.begin();
			}
			else {
				LOG(LOGTYPE_ERROR, "Texture index not found when batching");
			}
			batch_texture_ids.insert(texture_id);*/

			Vertex v1 = {
				p1.x, p1.y, p1.z,

				1,1,1,1,

				texco_u,
				texco_v,

				texture_index

			};

			Vertex v2 = {
				p2.x, p2.y, p2.z,

				1,1,1,1,

				0.f,
				texco_v,

				texture_index
			};

			Vertex v3 = {
				p3.x, p3.y, p3.z,

				1,1,1,1,

				0.0f,
				0.0f,

				texture_index
			};

			Vertex v4 = {
				p4.x, p4.y, p4.z,

				1,1,1,1,

				texco_u,
				0.0f,

				texture_index
			};

			buffer.push_back(v1);
			buffer.push_back(v2);
			buffer.push_back(v3);
			buffer.push_back(v4);

			if (batch_texture_ids.size() == max_layers)
			{
				//std::vector<Uint_32> batch_textures(batch_texture_ids.begin(), batch_texture_ids.end());
				make_batch(batch_texture_ids, usage);
			}

		}
		else if (usage == GL_STREAM_DRAW)
		{
			batch_graphic_ids.push_back(graphic_id);
			Uint_32 id_animator = graphics[graphic_id].animator_id;
			if (id_animator)
			{
				Animation& animation = animators[id_animator].get_current_animation();
				for (Keyframe& frame : animation.frames)
				{
					batch_textures_set.insert(frame.texture);
				}
			}
			else
			{
				auto t = Material::get_material(graphics[graphic_id].element.material)->diffuse_textures[0];
				batch_textures_set.insert(t);
			}

			if (batch_textures_set.size() >= max_layers)
			{
				std::vector<Uint_32> batch_textures(batch_textures_set.begin(), batch_textures_set.end());
				make_batch(batch_textures, usage);
			}
		}
		
		LOG(LOGTYPE_GENERAL, "Vertices processed: ", std::to_string(usage));


		
	}

	static bool compare(Uint_32 a, Uint_32 b )
	{
		auto t1 = Material::get_material(graphics[a].element.material)->diffuse_textures[0];
		auto t2 = Material::get_material(graphics[b].element.material)->diffuse_textures[0];
		
		if (t1 < t2)
			return true;
		else
			return false;
	}

	//void RendererOpenGL2D::prepare_dynmic_batches(std::vector<Uint_32> batched_graphic_ids)
	//{
	//	// get all unique textures from all animations
	//	std::set<Uint_32> texture_ids;
	//	for (Uint_32 i : batched_graphic_ids)
	//	{
	//		Uint_32 id_animator = graphics[i].id_animator;
	//		if (id_animator)
	//		{
	//			Animation& animation = animators[id_animator].animation;
	//			for (Keyframe& frame : animation.frames)
	//			{
	//				texture_ids.insert(frame.texture);
	//			}
	//		}
	//	}
	//	std::vector<Uint_32> texture_id_vec(texture_ids.begin(), texture_ids.end());

	//	for (auto i : batched_graphic_ids)
	//	{
	//		process_vertecies(i, texture_id_vec, GL_DYNAMIC_DRAW);
	//	}

	//	if (batch_texture_ids.size() > 0) // last batch may not have been created
	//	{
	//		std::vector<Uint_32> batch_textures(batch_texture_ids.begin(), batch_texture_ids.end());
	//		make_batch(batch_textures, GL_DYNAMIC_READ);
	//	}
	//}


	void RendererOpenGL2D::prepare_batches(std::vector<Uint_32> batched_graphic_ids, Uint_32 usage)
	{
		std::sort(batched_graphic_ids.begin(), batched_graphic_ids.end(), compare);
		for (auto i : batched_graphic_ids)
		{
			process_vertecies(i, usage);
		}

		// static
		if (batch_texture_ids.size() > 0) // last batch may not have been created
		{
			std::vector<Uint_32> batch_textures(batch_texture_ids.begin(), batch_texture_ids.end());
			make_batch(batch_textures, usage);
		}

		// dynamic
		if (batch_textures_set.size() > 0)
		{
			std::vector<Uint_32> batch_textures(batch_textures_set.begin(), batch_textures_set.end());
			make_batch(batch_textures, usage);
		}
	}

	void RendererOpenGL2D::generate_sprite_graphics(Graphic& graphic, Uint_32 graphic_id)
	{
		Uint_32 material_id = graphic.element.material;
		assert(material_id);

		Material* mat = Material::get_material(material_id);
		Texture* tex = Texture::get_texture(mat->diffuse_textures[0]); //Material::material_library[mat_id].diffuse_texture;
		Float_32 x_scale = tex->width;
		Float_32 y_scale = tex->height;
		if (x_scale > y_scale)
		{
			x_scale = (x_scale / y_scale);
			y_scale = 1.f;
		}
		else
		{
			y_scale = y_scale / x_scale;
			x_scale = 1.f;
		}

		if (graphic_id)
		{
			Float_32 import_scale = Graphic::editor_data[graphic_id].scale;
			x_scale *= import_scale;
			y_scale *= import_scale;
		}

		std::vector<GLuint> indices;
		std::vector<Vertex> buffer;

		Vertex v1 = {
			0.5f*x_scale,
			0.5f*y_scale,
			0.0f,

			1,1,1,1,

			1.0f,
			1.0f
		};

		Vertex v2 = {
			-0.5f*x_scale,
			0.5f*y_scale,
			0.f,

			1,1,1,1,

			0.f,
			1.0f
		};

		Vertex v3 = {
			-0.5f*x_scale,
			-0.5f*y_scale,
			0.f,

			1,1,1,1,

			0.0f,
			0.0f
		};

		Vertex v4 = {
			0.5f*x_scale,
			-0.5f*y_scale,
			0.f,

			1,1,1,1,

			1.0f,
			0.0f
		};

		buffer.push_back(v1);
		buffer.push_back(v2);
		buffer.push_back(v3);
		buffer.push_back(v4);

		if (graphic_id)
		{	// save vertex data for collider
			Graphic::vertex_data[graphic_id][0] = Vec3f{ v1.p_x, v1.p_y, v1.p_z };
			Graphic::vertex_data[graphic_id][1] = Vec3f{ v2.p_x, v2.p_y, v2.p_z };
			Graphic::vertex_data[graphic_id][2] = Vec3f{ v3.p_x, v3.p_y, v3.p_z };
			Graphic::vertex_data[graphic_id][3] = Vec3f{ v4.p_x, v4.p_y, v4.p_z };
		}

		indices.push_back(0);
		indices.push_back(1);
		indices.push_back(2);
		indices.push_back(2);
		indices.push_back(3);
		indices.push_back(0);

		VertexLayout layout;
		VertexAttribute attribute_0(0, 3, GL_FLOAT, GL_FALSE);
		VertexAttribute attribute_1(1, 4, GL_FLOAT, GL_FALSE);
		//VertexAttribute attribute_2(2,3,GL_FLOAT,GL_FALSE);
		VertexAttribute attribute_3(3, 2, GL_FLOAT, GL_FALSE);
		layout.add_attribute(attribute_0);
		layout.add_attribute(attribute_1);
		//layout.add_attribute(attribute_2);
		layout.add_attribute(attribute_3);

		//graphic.bounding_rect = Rect{0,0, x_scale, y_scale };
		graphic.element.vao.Generate();
		graphic.element.vbo.Generate(&buffer[0], buffer.size() * sizeof(Vertex), GL_STATIC_DRAW);
		graphic.element.layout = layout;
		for (Uint_32 _i = 0; _i < graphic.element.layout.next_attrib; _i++)
		{
			VertexAttribute* attr = &graphic.element.layout.vertex_attributes[_i];
			GL_CALL(
				glEnableVertexAttribArray(attr->index))
				GL_CALL(
					glVertexAttribPointer(attr->index, attr->count, attr->type, attr->normalized, layout.stride, (void*)attr->offset))
		}

		graphic.element.ibo.Generate(&indices[0], indices.size() * sizeof(GLuint), indices.size());
		graphic.element.num_of_triangles = (buffer.size() / 3);

		graphic.element.vao.Unbind();
		graphic.element.ibo.Unbind();
	}

	void RendererOpenGL2D::generate_sprite_graphics(Uint_32 graphic_id)
	{
		//Uint_32 mat_id = Material::load_material("shaders" + PATH_SEP + "DiffuseUnlit2D.shader", texture_file_path, mat_name);
		// find the proper scale needed for the quad mesh
		Graphic& graphic = graphics[graphic_id];
		generate_sprite_graphics(graphic, graphic_id);

    }


	void RendererOpenGL2D::draw_line(Vec3f p1, Vec3f p2, Vec4f color)
	{
		//return;
		lines.push_back({ color, p1,p2 });
	}

	

	void RendererOpenGL2D::draw_rect(Rect<Float_32> rect, Vec4f color)
	{
		Vec3f p1{ rect.x,rect.y,0};
		Vec3f p2{ rect.x,rect.y+rect.h,0 };
		Vec3f p3{ rect.x+rect.w,rect.y+rect.h, 0 };
		Vec3f p4{ rect.x+rect.w,rect.y ,0};
		renderer->draw_line(p1, p2, color);
		renderer->draw_line(p2, p3, color);
		renderer->draw_line(p3, p4, color);
		renderer->draw_line(p4, p1, color);
	}

	void RendererOpenGL2D::draw_rect_with_transform(Rect<Float_32> rect, Vec4f color, const Mat4x4& transformation)
	{
		Vec3f p1{ rect.x,rect.y,0 };
		p1 = transformation * p1;
		Vec3f p2{ rect.x,rect.y + rect.h,0 };
		p2 = transformation * p2;
		Vec3f p3{ rect.x + rect.w,rect.y + rect.h, 0 };
		p3 = transformation * p3;
		Vec3f p4{ rect.x + rect.w,rect.y ,0 };
		p4 = transformation * p4;
		renderer->draw_line(p1, p2, color);
		renderer->draw_line(p2, p3, color);
		renderer->draw_line(p3, p4, color);
		renderer->draw_line(p4, p1, color);
	}




    RenderLayer* RendererOpenGL2D::get_layer(const std::string& layer_name)
    {
        for(std::vector<RenderLayer*>::iterator it=render_layers.begin(); it!=render_layers.end(); it++)
        {
                if((*it)->name == layer_name){
                    return (*it);
                }
        }
        return nullptr;
    }

	void RendererOpenGL2D::render_graphic(Graphic& graphic, Mat4x4& transformation, Camera& _camera, DirectionalLight& _light)
	{

		GraphicsElement& element = graphic.element;
		Material* mat = Material::get_material(element.material);
		if (mat == nullptr)
		{
			LOG(LOGTYPE_ERROR, "Couldn't find material");
			return;
		}

		//graphic.bounding_rect.x = transform.position.x;
		//graphic.bounding_rect.y = transform.position.y;
		//std::cout<<"before: "<<grp->element.material.uniform_locations["u_MVP"]  <<std::endl;

		// only bind texture for the last texture unit. Ideally we want to have fewer units in use
		// to avoid binding all together. Assumption is that textures are bound at consturction and
		// remain bound
		Texture* tex = Texture::get_texture(mat->diffuse_textures[0]);
		//if (tex->bind_unit == max_texture_units - 1)
		mat->Bind();

		//glActiveTexture(GL_TEXTURE0 + tex->bind_unit);
		Shader* shader = Shader::get_shader(mat->shader);
		if (shader == nullptr)
		{
			LOG(LOGTYPE_ERROR, "Shader couldn't be found");
			return;
		}
		//glUseProgram(shader->id);
		GLint* uniform_loc = shader->uniform_locations;


		for (Uint_32 _i = 0; _i < (Int_32)ShaderUniformName::u_count; _i++)
		{
			ShaderUniformName _name = (ShaderUniformName)_i;
			GLint _loc = uniform_loc[_i];
			if (_loc >= 0)
			{
				switch (_name)
				{
				case ShaderUniformName::u_sampler2d:
				{
					Texture* tex = Texture::get_texture(mat->diffuse_textures[0]);
					//GL_CALL(
					glUniform1i(_loc, tex->bind_unit);//)
				}
				break;
				case ShaderUniformName::u_textures:	//happens with batche graphic, batche graphi will have array of texture ids
				{
					/*GLint texture_bind_units[MAX_TEXTURES] = {};
					int _count = 0;
					for (int _i = 0; _i < MAX_TEXTURES; _i++)
					{
						//if (mat->diffuse_textures[_i] != 0)
						{
							Texture* tex = Texture::get_texture(mat->diffuse_textures[_i]);
							texture_bind_units[_i] = tex->bind_unit;
							_count++;
						}
						//else
						//	break;
					}

					//GL_CALL(
					glUniform1iv(it.second.second, _count, texture_bind_units);//)*/
				}
				break;
				/*case ShaderUniformName::u_Dir_Light:
					GL_CALL(
						glUniform3f(it.second.second, _dir.x, _dir.y, _dir.z))
						break;*/
				case ShaderUniformName::u_Model:
					GL_CALL(
						glUniformMatrix4fv(_loc, 1, GL_FALSE, transformation.data))
						break;
				case ShaderUniformName::u_View:
					GL_CALL(
						glUniformMatrix4fv(_loc, 1, GL_FALSE, _camera.view_matrix.data))
						break;
				case ShaderUniformName::u_View_t:
				{
					Mat4x4 _view = Mat4x4(_camera.view_matrix);
					_view(0, 3) = 0;
					_view(1, 3) = 0;
					_view(2, 3) = 0;
					_view(3, 0) = 0;
					_view(3, 1) = 0;
					_view(3, 2) = 0;
					_view(3, 3) = 1;

					GL_CALL(
						glUniformMatrix4fv(_loc, 1, GL_FALSE, _view.data))
				}
				break;
				case ShaderUniformName::u_Projection:
					GL_CALL(
						glUniformMatrix4fv(_loc, 1, GL_FALSE, _camera.projection_matrix.data))
						break;
					/*case ShaderUniformName::u_Camera_Position:
						GL_CALL(
							glUniform3f(it.second.second, _cam_pos.x, _cam_pos.y, _cam_pos.z))
							break;
					case ShaderUniformName::u_Normal_M:
						GL_CALL(
							glUniformMatrix4fv(it.second.second, 1, GL_TRUE, transform.rotation_transformation.data))
							break;*/
				case ShaderUniformName::u_Panning:
					GL_CALL(
						glUniform2f(_loc, mat->panning.x, mat->panning.y))
						break;
				case ShaderUniformName::u_Tiling:
					GL_CALL(
						glUniform2f(_loc, mat->tiling.x, mat->tiling.y))
						break;
				case ShaderUniformName::u_Diffuse_Color:
					GL_CALL(
						glUniform3f(_loc, mat->diffuse_color.x, mat->diffuse_color.y, mat->diffuse_color.z))
						break;
				case ShaderUniformName::u_Outline_Color:
					GL_CALL(
						glUniform4f(_loc, graphic.outline_color.x, graphic.outline_color.y, graphic.outline_color.z, graphic.outline_alpha))
						break;
				case ShaderUniformName::u_Ambient_Strength:
					GL_CALL(
						glUniform1f(_loc, 1.f))
						break;
				default:
					break;
				}
			}



			//}

			//grp->ibo[i].Bind();

			//GL_CALL(
			//    glUniform1f((*it)->material.uniform_locations["u_red"], 1.f))
		}
		element.vao.Bind();
		element.ibo.Bind();
		GL_CALL(
			//glDrawArrays(GL_TRIANGLES,0, grp->element.num_of_triangles*3))
			glDrawElements(GL_TRIANGLES, element.ibo.count, GL_UNSIGNED_INT, nullptr));
		element.vao.Unbind();
		element.ibo.Unbind();
		mat->Unbind();

	}

/*
    void Renderer3D::upload_mesh(const Mesh& mesh)
    {

    }*/
}
