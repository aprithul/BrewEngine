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
#include "ImGuiConfiguration.hpp"
#include "Math.hpp"
#include <cmath>
#include <set>
#include <time.h>
#include <algorithm>

namespace PrEngine {

	RendererOpenGL2D* renderer = nullptr;
	//Uint_32 max_vertices_in_batch;
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
		//window = SDL_CreateWindow(this->title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_OPENGL);// | (full_screen*SDL_WINDOW_FULLSCREEN) );
		window = SDL_CreateWindow(this->title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);// | (full_screen*SDL_WINDOW_FULLSCREEN) );
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


        //data = stbi_load( get_resource_path("default.jpg").c_str() ,&width, &height, &no_of_channels, 0);


        /// create render layers
        //GeometryLayer* geometry_layer = new GeometryLayer(camera_handle);
        //render_layers.push_back(geometry_layer);

		//std::memset(BatchedGraphic::animation_to_batch_id, 0, sizeof(Uint_32)*Max_animation_count);

        SpriteLayer* sprite_layer = new SpriteLayer();
        render_layers.push_back(sprite_layer);

		ShapesLayer* shapes_layer = new ShapesLayer();
		render_layers.push_back(shapes_layer);

		//GizmoLayer* gizmo_layer = new GizmoLayer();
		//render_layers.push_back(gizmo_layer);

#ifdef EDITOR_MODE
		EditorLayer* editor_layer = new EditorLayer(window, &glContext);
		render_layers.push_back(editor_layer);
#endif // EDITOR_MODE

		renderer = this;
		std::memset(is_included_at_index, -1, sizeof(Int_32) * 99999);
		batch_pool = new BatchPool(100);

		initialize_imgui(window, glContext);

		GL_CALL(glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &max_texture_units));
		GL_CALL(glGetIntegerv(GL_MAX_ARRAY_TEXTURE_LAYERS, &max_layers));	//max depth of array textures
	
		//max_layers = 9;

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
        Clear(0.2f,0.2f,0.2f,1);
		clock_t begin = clock();

		for (std::vector<RenderLayer*>::iterator layer = render_layers.begin(); layer != render_layers.end(); layer++)
		{
			(*layer)->update();
		}
        SwapBuffers();
		clock_t end = clock();
		Double_64 elapsed = (Double_64)(end - begin) / CLOCKS_PER_SEC;
		batch_pool->return_all_batches();
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
		//Uint_32 entity = entity_management_system->make_entity();
		//if (entity)
		//{
		//	VertexLayout layout;
		//	VertexAttribute attribute_0(0, 3, GL_FLOAT, GL_FALSE);
		//	VertexAttribute attribute_1(1, 4, GL_FLOAT, GL_FALSE);
		//	//VertexAttribute attribute_2(2, 3, GL_FLOAT, GL_FALSE);
		//	VertexAttribute attribute_3(2, 2, GL_FLOAT, GL_FALSE);
		//	VertexAttribute attribute_4(3, 1, GL_FLOAT, GL_FALSE);

		//	layout.add_attribute(attribute_0);
		//	layout.add_attribute(attribute_1);
		//	//layout.add_attribute(attribute_2);
		//	layout.add_attribute(attribute_3);
		//	layout.add_attribute(attribute_4);

		//	//batched_graphics.emplace_back();
		//	//auto& batch = batched_graphics.back();

		//	Uint_32 batc_grap_id = batched_graphics_system.make(0); // we need a dummy entity, so using entity 0

		//	BatchedGraphic& batch = batched_graphics_system.get_component(batc_grap_id);
		//	LOG(LOGTYPE_WARNING, "load batch material");
		//	batch.element.material = Material::load_material("Materials" + PATH_SEP + "Batch.mat", false, std::to_string(batch_count++));
		//	//next_batched_graphic_pos++;
		//	batch.transform_id = 0;	// use identity unless modified later ( eg. animations)

		//	if (usage == GL_STATIC_DRAW)
		//		batch.tag = RENDER_STATIC;
		//	else if (usage == GL_STREAM_DRAW)
		//	{
		//		batch.tag = RENDER_DYNAMIC;
		//		batch.graphic_ids = batch_graphic_ids;
		//		// store texture indices for dynamic vbo 
		//		for (int _i = 0; _i < texture_ids.size(); _i++)
		//		{
		//			batch.texture_to_index[texture_ids[_i]] = _i;
		//		}

		//	}

		//	Material* mat = Material::get_material(batch.element.material);

		//	//memcpy(mat->diffuse_textures, &texture_ids[0], sizeof(Uint_32) * texture_ids.size());

		//	// make array texture
		//	mat->diffuse_textures[0] = Texture::make_array_texture(texture_ids);

		//	batch.element.vao.Generate();
		//	LOG(LOGTYPE_WARNING, "vao generated: ", std::to_string(batch.element.vao.id));
		//	if (usage == GL_STATIC_DRAW)
		//	{
		//		batch.element.vbo.Generate(&buffer[0], buffer.size() * sizeof(Vertex), GL_STATIC_DRAW);
		//	}
		//	else
		//	{
		//		batch.element.vbo.Generate(nullptr, sizeof(Vertex) * BatchedGraphic::max_vertices_in_batch, GL_STREAM_DRAW);
		//	}
		//	LOG(LOGTYPE_WARNING, "vbo generated");

		//	batch.element.layout = layout;
		//	LOG(LOGTYPE_WARNING, "layout generated");

		//	for (Uint_32 _i = 0; _i < batch.element.layout.next_attrib; _i++)// != batch.element.layout.vertex_attributes.end(); attr++)
		//	{
		//		VertexAttribute* attr = &(batch.element.layout.vertex_attributes[_i]);
		//		GL_CALL(
		//			glEnableVertexAttribArray(attr->index))
		//			//GL_CALL(
		//			glVertexAttribPointer(attr->index, attr->count, attr->type, attr->normalized, layout.stride, (void*)attr->offset);//)
		//	}

		//	batch.element.ibo.Generate(&indices[0], indices.size() * sizeof(GLuint), indices.size());
		//	batch.element.num_of_triangles = (buffer.size() / 3);
		//	LOG(LOGTYPE_WARNING, "ibo generated");

		//	batch.element.vao.Unbind();
		//	batch.element.ibo.Unbind();


		//	buffer.clear();
		//	indices.clear();
		//	batch_texture_ids.clear();
		//	batch_textures_set.clear();
		//	batch_graphic_ids.clear();
		//	index = 0;

		//	LOG(LOGTYPE_GENERAL, "Batch made: ", std::to_string(usage));
		//}

	}


	void process_vertecies(Uint_32 graphic_id, Uint_32 usage)
	{

		//indices.push_back(index);
		//indices.push_back(index + 1);
		//indices.push_back(index + 2);
		//indices.push_back(index + 2);
		//indices.push_back(index + 3);
		//indices.push_back(index);
		//index += 4;


		//Graphic& graphic = graphics_system.get_component(graphic_id);// graphics[graphic_id];
		//Uint_32 material_id = graphic.element.material;
		//assert(material_id);
		//Material* mat = Material::get_material(material_id);
		//Texture* tex = Texture::get_texture(mat->diffuse_textures[0]); //Material::material_library[mat_id].diffuse_texture;
		//Float_32 x_scale = tex->width;
		//Float_32 y_scale = tex->height;
		//Float_32 texco_u = clamp(tex->width / (Float_32)MAX_TEXTURE_SIZE, 0.1f, 1.0f);
		//Float_32 texco_v = clamp(tex->height / (Float_32)MAX_TEXTURE_SIZE, 0.1f, 1.0f);

		//if (x_scale > y_scale)
		//{
		//	x_scale = (x_scale / y_scale);
		//	y_scale = 1.f;
		//}
		//else
		//{
		//	y_scale = y_scale / x_scale;
		//	x_scale = 1.f;
		//}
		//GraphicEditorData ged = Graphic::editor_data[graphic_id];
		//x_scale *= ged.scale;
		//y_scale *= ged.scale;

		//Transform3D& transform = transform_system.get_component(graphic.transform_id);
		//Vec3f p1 =  Vec3f{ 0.5f*x_scale, 0.5f*y_scale, 0.0f };
		//Vec3f p2 =  Vec3f{ -0.5f*x_scale, 0.5f*y_scale, 0.0f };
		//Vec3f p3 =  Vec3f{ -0.5f*x_scale, -0.5f*y_scale, 0.0f };
		//Vec3f p4 =  Vec3f{ 0.5f*x_scale, -0.5f*y_scale, 0.0f };

		//// save vertex data for collider
		//Graphic::vertex_data[graphic_id][0] = p1;
		//Graphic::vertex_data[graphic_id][1] = p2;
		//Graphic::vertex_data[graphic_id][2] = p3;
		//Graphic::vertex_data[graphic_id][3] = p4;

		//if (usage == GL_STATIC_DRAW)
		//{
		//	p1 = transform.transformation * p1;
		//	p2 = transform.transformation * p2;
		//	p3 = transform.transformation * p3;
		//	p4 = transform.transformation * p4;

		//	Uint_32 texture_id = mat->diffuse_textures[0];
		//	if (batch_texture_ids.size() == 0 || batch_texture_ids.back() != texture_id)
		//	{
		//		batch_texture_ids.push_back(texture_id);
		//	}
		//	Float_32 texture_index = batch_texture_ids.size() - 1;
		//	// find index of texture in set

		//	/*auto it = std::find(texture_ids.begin(), texture_ids.end(), texture_id);
		//	if (it != texture_ids.end())
		//	{
		//		texture_index = it - texture_ids.begin();
		//	}
		//	else {
		//		LOG(LOGTYPE_ERROR, "Texture index not found when batching");
		//	}
		//	batch_texture_ids.insert(texture_id);*/

		//	Vertex v1 = {
		//		p1.x, p1.y, p1.z,

		//		1,1,1,1,

		//		texco_u,
		//		texco_v,

		//		texture_index

		//	};

		//	Vertex v2 = {
		//		p2.x, p2.y, p2.z,

		//		1,1,1,1,

		//		0.f,
		//		texco_v,

		//		texture_index
		//	};

		//	Vertex v3 = {
		//		p3.x, p3.y, p3.z,

		//		1,1,1,1,

		//		0.0f,
		//		0.0f,

		//		texture_index
		//	};

		//	Vertex v4 = {
		//		p4.x, p4.y, p4.z,

		//		1,1,1,1,

		//		texco_u,
		//		0.0f,

		//		texture_index
		//	};

		//	buffer.push_back(v1);
		//	buffer.push_back(v2);
		//	buffer.push_back(v3);
		//	buffer.push_back(v4);

		//	if (batch_texture_ids.size() == max_layers)
		//	{
		//		//std::vector<Uint_32> batch_textures(batch_texture_ids.begin(), batch_texture_ids.end());
		//		make_batch(batch_texture_ids, usage);
		//	}

		//}
		//else if (usage == GL_STREAM_DRAW)
		//{
		//	batch_graphic_ids.push_back(graphic_id);
		//	Uint_32 id_animator = graphics_system.get_component(graphic_id).animator_id;
		//	if (id_animator)
		//	{
		//		//Animation& animation = animators[id_animator].get_current_animation();
		//		Animation& animation = animator_system.get_component(id_animator).get_current_animation();
		//		for (Keyframe& frame : animation.frames)
		//		{
		//			batch_textures_set.insert(frame.texture);
		//		}
		//	}
		//	else
		//	{
		//		auto t = Material::get_material(graphics_system.get_component(graphic_id).element.material)->diffuse_textures[0];
		//		batch_textures_set.insert(t);
		//	}

		//	if (batch_textures_set.size() >= max_layers)
		//	{
		//		std::vector<Uint_32> batch_textures(batch_textures_set.begin(), batch_textures_set.end());
		//		make_batch(batch_textures, usage);
		//	}
		//}
		//
		//LOG(LOGTYPE_GENERAL, "Vertices processed: ", std::to_string(usage));


		
	}

	static bool compare(Uint_32 a, Uint_32 b )
	{
		auto t1 = Material::get_material(graphics_system.get_component(a).element.material)->diffuse_textures[0];
		auto t2 = Material::get_material(graphics_system.get_component(b).element.material)->diffuse_textures[0];
		
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

	void generate_array_texture(Uint_32 batch_id, std::vector<Uint_32>& texture_ids)
	{
		BatchedGraphic& last_batch = batched_graphics_system.get_component(batch_id);
		Uint_32 arr_tex_id = Texture::make_array_texture(texture_ids);
		Material::get_material(last_batch.element.material)->diffuse_textures[0] = arr_tex_id;
	}
	void RendererOpenGL2D::add_to_batch(std::vector< Uint_32>& graphic_ids)
	{
		//static Uint32 last_batch_id = 0;
		//static std::vector<Uint_32> texture_ids;

		//for (auto graphic_id : graphic_ids)
		//{
		//	Graphic& graphic = graphics_system.get_component(graphic_id);
		//	if (graphic.animator_id)
		//	{
		//		Animator& animator = animator_system.get_component(graphic.animator_id);
		//		for (Uint_32 _i = 0; _i < animator.animation_count; _i++)
		//		{

		//			Uint_32 animation_id = animator.animation_ids[_i];
		//			if (animation_id)
		//			{
		//				Animation& animation = animator.get_animation(animation_id);
		//				Uint_32 anim_frame_count = animation.frames.size();

		//				Uint_32 batch_id = BatchedGraphic::animation_to_batch_id[animation_id];
		//				if (batch_id)
		//				{
		//					BatchedGraphic& batch = batched_graphics_system.get_component(batch_id);
		//					if (batch.current_batched_vertex_count <= batch.max_vertices_in_batch - 4)
		//					{
		//						if (graphic.batch_id != batch_id)
		//						{
		//							graphic.batch_id = batch_id;
		//							batch.graphic_ids.push_back(graphic_id);
		//						}
		//						//batch.current_batched_vertex_count += 4;
		//						last_batch_id = batch_id;
		//						//return;
		//					}
		//					else
		//					{
		//						if (last_batch_id)
		//						{
		//							if (texture_ids.size() > 0)
		//							{
		//								generate_array_texture(last_batch_id, texture_ids);
		//								texture_ids.clear();
		//							}
		//						}

		//						Uint_32 new_batch_id = batched_graphics_system.make(0);
		//						BatchedGraphic& new_batch = batched_graphics_system.get_component(new_batch_id);
		//						new_batch.tag = RENDER_DYNAMIC;
		//						new_batch.initialize(new_batch_id);
		//						Material::get_material(new_batch.element.material)->diffuse_textures[0] = Material::get_material(batch.element.material)->diffuse_textures[0];
		//						std::memcpy(new_batch.texture_to_index, batch.texture_to_index, 99999);
		//						if (graphic.batch_id != new_batch_id)
		//						{
		//							graphic.batch_id = new_batch_id;
		//							new_batch.graphic_ids.push_back(graphic_id);
		//						}
		//						//new_batch.current_batched_vertex_count += 4;
		//						last_batch_id = new_batch_id;
		//					}
		//				}
		//				else
		//				{
		//					if (last_batch_id)
		//					{

		//						BatchedGraphic& last_batch = batched_graphics_system.get_component(last_batch_id);
		//						if (last_batch.current_batched_vertex_count >= last_batch.max_vertices_in_batch - 4 ||
		//							last_batch.current_batched_texture_count >= max_layers - anim_frame_count)
		//						{
		//							if (texture_ids.size() > 0)
		//							{
		//								generate_array_texture(last_batch_id, texture_ids);
		//								texture_ids.clear();
		//							}

		//							Uint_32 new_batch_id = batched_graphics_system.make(0);
		//							BatchedGraphic& new_batch = batched_graphics_system.get_component(new_batch_id);
		//							new_batch.tag = RENDER_DYNAMIC;
		//							new_batch.initialize(new_batch_id);

		//							last_batch = new_batch;
		//							last_batch_id = new_batch_id;
		//						}

		//						if (graphic.batch_id != last_batch_id)
		//						{
		//							graphic.batch_id = last_batch_id;
		//							last_batch.graphic_ids.push_back(graphic_id);
		//						}
		//						//last_batch.current_batched_vertex_count += 4;
		//						last_batch.current_batched_texture_count += anim_frame_count;
		//						for (auto& frame : animation.frames)
		//						{
		//							texture_ids.push_back(frame.texture);
		//							last_batch.texture_to_index[frame.texture] = texture_ids.size() - 1;
		//						}

		//					}
		//					else
		//					{
		//						Uint_32 new_batch_id = batched_graphics_system.make(0);
		//						BatchedGraphic& new_batch = batched_graphics_system.get_component(new_batch_id);
		//						new_batch.tag = RENDER_DYNAMIC;
		//						new_batch.initialize(batch_id);

		//						if (graphic.batch_id != new_batch_id)
		//						{
		//							graphic.batch_id = new_batch_id;
		//							new_batch.graphic_ids.push_back(graphic_id);
		//						}
		//						//new_batch.current_batched_vertex_count += 4;
		//						new_batch.current_batched_texture_count += anim_frame_count;
		//						last_batch_id = new_batch_id;
		//						for (auto& frame : animation.frames)
		//						{
		//							texture_ids.push_back(frame.texture);
		//							new_batch.texture_to_index[frame.texture] = texture_ids.size() - 1;

		//						}
		//					}
		//				}

		//				if (BatchedGraphic::animation_to_batch_id[animation_id] == 0)
		//					BatchedGraphic::animation_to_batch_id[animation_id] = last_batch_id;


		//			}
		//		}
		//	}
		//
		//	batched_graphics_system.get_component(last_batch_id).current_batched_vertex_count += 4;
		//}

		//if (texture_ids.size() > 0)
		//{
		//	generate_array_texture(last_batch_id, texture_ids);
		//	texture_ids.clear();
		//}


	}

	BatchedGraphic* RendererOpenGL2D::make_dynamic_batch(std::vector<Uint_32>& graphic_ids)
	{
		BatchedGraphic& batch = batched_graphics_system.get_component(0);
		batch_textures_set.clear();

		// sort graphics ids by current animation so that same animation graphics are together
		auto sort_lambda = [](Uint_32 a, Uint_32 b)
		{
			Uint_32 id_animator_a = graphics_system.get_component(a).animator_id;
			Uint_32 id_animator_b = graphics_system.get_component(b).animator_id;

			Animator& animt_a = animator_system.get_component(id_animator_a);
			Animator& animt_b = animator_system.get_component(id_animator_b);

			Int_32 ind_a = animt_a.cur_anim_ind;
			Int_32 ind_b = animt_b.cur_anim_ind;

			return  animt_a.animation_ids[ind_a] >= animt_a.animation_ids[ind_b];
		};
		std::sort(graphic_ids.begin(), graphic_ids.end(), sort_lambda);


		//Uint_32 current_batch_remaining_layers = max_layers;
		//Int_32 last_anim_ind = -1;
		for (auto _id : graphic_ids)
		{
			Uint_32 id_animator = graphics_system.get_component(_id).animator_id;
			if (id_animator)
			{
				Animator& animtr = animator_system.get_component(id_animator);
				Animation& animation = animtr.get_current_animation();
				Int_32 cur_anim_id = animtr.animation_ids[animtr.cur_anim_ind];

				for (Keyframe& frame : animation.frames)
				{
					batch_textures_set.insert(frame.texture);
				}
			}
			else
			{
				auto t = Material::get_material(graphics_system.get_component(_id).element.material)->diffuse_textures[0];
				batch_textures_set.insert(t);
			}
		}
		return &batch;
	}


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
		Graphic& graphic = graphics_system.get_component(graphic_id);
		generate_sprite_graphics(graphic, graphic_id);

    }


	void RendererOpenGL2D::draw_line(Vec3f p1, Vec3f p2, Vec4f color)
	{
		//return;
		lines.push_back({ color, p1,p2 });
	}

	void RendererOpenGL2D::draw_ray(Vec3f origin, Vec3f dir, Float_32 len, Vec4f color)
	{
		Vec3f p2 = origin + (dir*len);
		lines.push_back({ color, origin, p2});

	}

	

	void RendererOpenGL2D::draw_line_with_transform(Vec3f p1, Vec3f p2, Vec4f color, const Mat4x4& transformation)
	{
		//return;
		p1 = transformation * p1;
		p2 = transformation * p2;

		lines.push_back({ color, p1,p2 });
	}

	void RendererOpenGL2D::draw_point(Vec3f pos)
	{
		Vec2f points[4];
		points[0] = pos + Vec2f{ 0.01f, 0.01f };
		points[1] = pos + Vec2f{ -0.01f, 0.01f };
		points[2] = pos + Vec2f{ -0.01f, -0.01f };
		points[3] = pos + Vec2f{ 0.01f, -0.01f };
		Rect<Float_32> _contact_point = points_to_rect(points);
		renderer->draw_rect(_contact_point, { 1,1,1,1 });
	}

	void RendererOpenGL2D::draw_shape(Vec2f* points, Uint_32 point_count, const Vec4f& color)
	{
		for (Uint_32 _i = 1; _i < point_count; _i++)
		{
			draw_line(points[_i - 1], points[_i], color);
		}
		draw_line(points[point_count-1], points[0], color);
	}

	void RendererOpenGL2D::draw_shape_with_transform(Vec2f* points, Uint_32 point_count, const Mat4x4& transformation, const Vec4f& color)
	{
		Vec2f transformed_points[16];
		for (Uint_32 _i = 0; _i < point_count; _i++)
		{
			transformed_points[_i] = transformation * points[_i];
		}

		draw_shape(transformed_points, point_count, color);
	}

	void RendererOpenGL2D::draw_circle(Vec2f origin, Float_32 radius, Vec4f color)
	{
		const static int sides = 16;
		const static Float_32 angle_step = (2 * PI) / sides;
		Vec2f shape_points[sides];
		for (Uint_32 _i = 0; _i < sides; _i++)
		{
			Float_32 _x = std::cosf(_i*angle_step);
			Float_32 _y = std::sinf(_i*angle_step);

			Vec2f point = { _x, _y };
			LOG(LOGTYPE_GENERAL, std::to_string(point.x), ",", std::to_string(point.y));
			shape_points[_i] = origin + (point * radius);
		}

		draw_shape(shape_points, sides, color);
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

	void RendererOpenGL2D::render_graphic(const Graphic& graphic, Uint_32 element_count, Mat4x4& transformation, Camera& _camera)
	{

		const GraphicsElement& element = graphic.element;
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
				case ShaderUniformName::u_sampler2darr0:
				{
					Texture* tex = Texture::get_texture(mat->diffuse_textures[0]);
					//GL_CALL(
					glUniform1i(_loc, tex->bind_unit);//)
				}
				break;
				case ShaderUniformName::u_sampler2darr1:
				{
					Texture* tex = Texture::get_texture(mat->diffuse_textures[1]);
					//GL_CALL(
					glUniform1i(_loc, tex->bind_unit);//)
				}
				break;
				case ShaderUniformName::u_sampler2darr2:
				{
					Texture* tex = Texture::get_texture(mat->diffuse_textures[2]);
					//GL_CALL(
					glUniform1i(_loc, tex->bind_unit);//)
				}
				break;
				case ShaderUniformName::u_sampler2darr3:
				{
					Texture* tex = Texture::get_texture(mat->diffuse_textures[3]);
					//GL_CALL(
					glUniform1i(_loc, tex->bind_unit);//)
				}
				break;
				case ShaderUniformName::u_sampler2darr4:
				{
					Texture* tex = Texture::get_texture(mat->diffuse_textures[4]);
					//GL_CALL(
					glUniform1i(_loc, tex->bind_unit);//)
				}
				break;
				case ShaderUniformName::u_sampler2darr5:
				{
					Texture* tex = Texture::get_texture(mat->diffuse_textures[5]);
					//GL_CALL(
					glUniform1i(_loc, tex->bind_unit);//)
				}
				break;
				case ShaderUniformName::u_sampler2darr6:
				{
					Texture* tex = Texture::get_texture(mat->diffuse_textures[6]);
					//GL_CALL(
					glUniform1i(_loc, tex->bind_unit);//)
				}
				break;
				case ShaderUniformName::u_sampler2darr7:
				{
					Texture* tex = Texture::get_texture(mat->diffuse_textures[7]);
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
			glDrawElements(GL_TRIANGLES, element_count, GL_UNSIGNED_INT, nullptr));
		element.vao.Unbind();
		element.ibo.Unbind();
		mat->Unbind();

	}


	// get all graphic_ids during scene loading and create array textures with their textures
	// of max possible size (dictated by max_layers)
	// no individual texture is duplicated

	void RendererOpenGL2D::prepare_array_textures(std::vector<Uint_32>& texture_ids)
	{

		static std::vector<Uint_32> unique_tex_ids;
		unique_tex_ids.push_back(0); // default texture
		is_included_at_index[0] = unique_tex_ids.size() - 1;

		//get vector of unique textures
		for (Uint_32 t_id : texture_ids)
		{
			if (is_included_at_index[t_id] == -1)
			{
				unique_tex_ids.push_back(t_id);
				is_included_at_index[t_id] = unique_tex_ids.size()-1;

			}
		}


		//for (Uint_32 g_id : graphic_ids)
		//{
		//	Uint_32 ent = graphics_system.get_entity(g_id);
		//	Uint_32 a_id = animator_system.get_component_id(ent);
		//	if (a_id)
		//	{
		//		Animator& animator = animator_system.get_component(a_id);
		//		for (Uint_32 anim_id : animator.animation_ids)
		//		{
		//			if (anim_id)
		//			{
		//				Animation& animation = animator.get_animation(anim_id);
		//				for (Keyframe frame : animation.frames)
		//				{
		//					if (is_included_at_index[frame.texture] == -1)
		//					{
		//						tex_ids.push_back(frame.texture);
		//						is_included_at_index[frame.texture] = tex_ids.size()-1;
		//					}
		//				}
		//			}
		//		}
		//	}
		//	else
		//	{
		//		Material* mat = Material::get_material(graphics_system.get_component(g_id).element.material);
		//		if (is_included_at_index[mat->diffuse_textures[0]] == -1)
		//		{
		//			tex_ids.push_back(mat->diffuse_textures[0]);
		//			is_included_at_index[mat->diffuse_textures[0]] = tex_ids.size()-1;
		//		}
		//	}
		//}

		// make texture from tex_ids
		for (Uint_32 _i = 0; _i < unique_tex_ids.size(); _i+=max_layers)
		{
			std::vector<Uint_32>::iterator start = unique_tex_ids.begin() + _i;
			std::vector<Uint_32>::iterator end = unique_tex_ids.begin() + _i + max_layers;
			if (_i + max_layers > unique_tex_ids.size())
				end = unique_tex_ids.end();

			std::vector<Uint_32> tex_id_for_tex_arr(start, end);
			Uint_32 array_tex = Texture::make_array_texture(tex_id_for_tex_arr);
			array_textures.push_back(array_tex);
		}
	}

	static std::vector<Uint_32> current_dynamic_batches;
	static std::vector<Uint_32> current_static_batches;
	void RendererOpenGL2D::add_to_batch(Uint_32 graphic_id, RenderTag render_mode)
	{
		

		// get all textures referenced from graphic_id and it's animations
		Uint_32 tex_id = 0;
		Graphic& g = graphics_system.get_component(graphic_id);
		Material* mat = Material::get_material(g.element.material);
		tex_id = mat->diffuse_textures[0];
		if(g.animator_id)
		{
			Animator& animator = animator_system.get_component(g.animator_id);
			Animation& animation = animator.get_current_animation();
			if(animation.frames.size() > animator.current_frame_index)
				tex_id = animation.frames[animator.current_frame_index].texture;
			//for (Uint_32 _i = 0; _i < 8; _i++)
			//{
			//	for (Uint_32 anim_id : animator.animation_ids)
			//	{
			//		if (anim_id)
			//		{
			//			Animation& animation = animator.get_animation(anim_id);
			//			for (Keyframe frame : animation.frames)
			//			{
			//				tex_ids.push_back(frame.texture);
			//			}
			//		}
			//	}
			//}
		}

		// which array texture contains this tex_id?
		Int_32 index = is_included_at_index[tex_id];
		if (index != -1)
		{
			Uint_32 array_tex_index = (index) / max_layers;
			Uint_32 index_in_arr_tex = (index) % max_layers;


			std::vector<Uint_32>& current_batches = current_dynamic_batches;
			if (render_mode == RENDER_STATIC)
				current_batches = current_static_batches;

			// try adding graphic to an existing batch
			for (Uint_32 batch_id : current_batches)
			{
				BatchedGraphic& batch = batched_graphics_system.get_component(batch_id);
				Material* mat = Material::get_material(batch.element.material);
				Uint_32 next_empty_tex_slot = -1;

				for (Uint_32 _i = 0; _i < 8; _i++)
				{
					// empty slots start here?
					if (mat->diffuse_textures[_i] == -1)
					{
						next_empty_tex_slot = _i;
						break;
					}	
					// texture available, is it the one we need?
					else if (mat->diffuse_textures[_i] == array_textures[array_tex_index])
					{
						if (batch.current_batched_vertex_count < BatchedGraphic::max_vertices_in_batch)
						{
							batch.graphic_ids.push_back({ graphic_id, _i });
							batch.current_batched_vertex_count += 4;
							return;
						}
						else
							break;
					}
				}

				// at least one empty tex slot is present in batch, so put our required texture there
				if (next_empty_tex_slot != -1 && batch.current_batched_vertex_count < BatchedGraphic::max_vertices_in_batch)
				{
					mat->diffuse_textures[next_empty_tex_slot] = array_textures[array_tex_index];
					batch.graphic_ids.push_back({ graphic_id, next_empty_tex_slot });
					batch.current_batched_vertex_count += 4;
					return;
				}

			}

			// couldn't add to any available batches. 
			// so put it in a new one.
			Uint_32 new_batch = 0;
			if (render_mode == RENDER_DYNAMIC)
			{
				new_batch = batch_pool->get_new_batch();
				current_dynamic_batches.push_back(new_batch);
			}
			else if (render_mode == RENDER_STATIC)
			{
				new_batch = batched_graphics_system.make(0);
				BatchedGraphic::initialize(new_batch, RENDER_STATIC);
				current_static_batches.push_back(new_batch);
			}

			BatchedGraphic& batch = batched_graphics_system.get_component(new_batch);
			batch.tag = render_mode;
			Material* mat = Material::get_material(batch.element.material);
			mat->diffuse_textures[0] = array_textures[array_tex_index];
			batch.graphic_ids.push_back({ graphic_id, 0 });
			batch.current_batched_vertex_count += 4;
			return;
		}
		else
		{
			// assert for debugging. 
			LOG(LOGTYPE_ERROR, "Fatal Error: Must be in one of the array textures.");
			assert(1);
		}
	}

	void RendererOpenGL2D::add_to_static_batch(std::vector<Uint_32>& graphic_ids)
	{
		/*static Uint_32 last_batch_id = 0;

		for (Uint_32 g_id : graphic_ids)
		{
			if (g_id)
			{
				Graphic& graphic = graphics_system.get_component(g_id);
				if (graphic.tag == RENDER_STATIC)
				{
					Uint_32 tex_id = 0;
					Material* mat = Material::get_material(graphic.element.material);
					tex_id = mat->diffuse_textures[0];

				}



			}
		}*/
	}

	BatchPool::BatchPool(Uint_32 pool_size)
	{
		this->pool_size = pool_size;
		pool = new Uint_32[pool_size];
		batch_in_use = new Bool_8[pool_size];
		for(Uint_32 _i=0; _i<pool_size; _i++)
		{
			Uint_32 batch_id = batched_graphics_system.make(0);
			BatchedGraphic& batch = batched_graphics_system.get_component(batch_id);
			batch.tag = RENDER_DYNAMIC;

			assert(batch_id);
			BatchedGraphic::initialize(batch_id, RENDER_DYNAMIC);

			pool[_i] = batch_id;
			batch_in_use[_i] = false;
		}
	}

	BatchPool::~BatchPool()
	{
		//
		delete[] pool;
		delete[] batch_in_use;
	}

	Uint_32 BatchPool::get_new_batch()
	{
		for (Uint_32 _i = 0; _i < pool_size; _i++)
		{
			if (batch_in_use[_i] == false)
			{
				batch_in_use[_i] = true;
				return pool[_i];
			}
		}

		LOG(LOGTYPE_ERROR, "No batchs in pool available");
		return 0;
	}

	void BatchPool::return_batch(Uint_32 batch_id)
	{
		for (Uint_32 _i = 0; _i < pool_size; _i++)
		{
			if (batch_id == pool[_i])
			{
				BatchedGraphic& batch = batched_graphics_system.get_component(batch_id);
				Material* mat = Material::get_material(batch.element.material);
				std::memset(mat->diffuse_textures, -1, sizeof(Uint_32) * 8);
				batch_in_use[_i] = false;
			}
		}
	}

	void BatchPool::return_all_batches()
	{
		current_dynamic_batches.clear();
		for (Uint_32 _i = 0; _i < pool_size; _i++)
		{
			if (batch_in_use[_i])
			{
				batch_in_use[_i] = false;
				BatchedGraphic& batch = batched_graphics_system.get_component(pool[_i]);
				batch.current_batched_texture_count = 0;
				batch.current_batched_vertex_count = 0;
				batch.graphic_ids.clear();
				Material* mat = Material::get_material(batch.element.material);
				std::memset(mat->diffuse_textures, -1, sizeof(Uint_32) * 8);
			}
		}
	}



/*
    void Renderer3D::upload_mesh(const Mesh& mesh)
    {

    }*/
}
