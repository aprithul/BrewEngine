//
//  Display.cpp
//  Game Engine
//
//  Created by Aniruddha Prithul on 7/22/17.
//  Copyright © 2017 Aniruddha Prithul. All rights reserved.
//
#include "RendererOpenGL2D.hpp"
#include "Math.hpp"
#include <cmath>
#include <set>
namespace PrEngine {

	RendererOpenGL2D* renderer = nullptr;
	GLint max_layers;
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

		/*this->viewport_x = width * 0.7 / 6;
		this->viewport_y = height / 4;
		this->viewport_width = width * 4.3 / 6;
		this->viewport_height = height * 3 / 4;*/
        //light_direction =
        // initialize sdl and opengl related settings for graphics
        init();

        // create window
        window = SDL_CreateWindow(this->title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, this->width, this->height, 
																														SDL_WINDOW_OPENGL | (full_screen*SDL_WINDOW_FULLSCREEN) );
        // create the openGL context from the window  that was created
        glContext = SDL_GL_CreateContext(window);

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

        if( glewInit() != GLEW_OK)
            printf("Glew not initialized properly");

        // if context creation failed, log the error
        if(glContext==NULL)
        {
            printf("SDL_Init failed: %s\n", SDL_GetError());
        }
        else
            printf("Context created with OpenGL version  (%s)\n", glGetString(GL_VERSION));

        std::cout<<"Vector3 size"<<sizeof(Vector3<Float_32>)<<std::endl;

        std::cout<<"Loading default texture"<<std::endl;
        //Texture::load_default_texture();
		Uint_32 mat = Material::load_material("Materials\\Default.mat", true);
		
		assert(Material::material_creation_status); // default material has to be created for engine to work

        //data = stbi_load( get_resource_path("default.jpg").c_str() ,&width, &height, &no_of_channels, 0);


        /// create render layers
        //GeometryLayer* geometry_layer = new GeometryLayer(camera_handle);
        //render_layers.push_back(geometry_layer);

        SpriteLayer* sprite_layer = new SpriteLayer();
        render_layers.push_back(sprite_layer);

        GuiLayer* gui_layer = new GuiLayer(window, &glContext);
        render_layers.push_back(gui_layer);

		ShapesLayer* shapes_layer = new ShapesLayer();
		render_layers.push_back(shapes_layer);


		renderer = this;

		
		glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &max_texture_units);
		glGetIntegerv(GL_MAX_ARRAY_TEXTURE_LAYERS, &max_layers);	//max depth of array textures
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
		glViewport(x, y, width, height);
	}

    void RendererOpenGL2D::init()
    {
        SDL_Init(SDL_INIT_VIDEO);
        //TTF_Init();

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
        Clear(0,0,0.3f,1);

        for(std::vector<RenderLayer*>::iterator layer=render_layers.begin(); layer!=render_layers.end(); layer++)
            (*layer)->update();
        SwapBuffers();
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
		VertexAttribute attribute_2(2, 3, GL_FLOAT, GL_FALSE);
		VertexAttribute attribute_3(3, 2, GL_FLOAT, GL_FALSE);
		VertexAttribute attribute_4(4, 1, GL_FLOAT, GL_FALSE);

		layout.add_attribute(attribute_0);
		layout.add_attribute(attribute_1);
		layout.add_attribute(attribute_2);
		layout.add_attribute(attribute_3);
		layout.add_attribute(attribute_4);

		batched_graphics.emplace_back();
		auto& batch = batched_graphics.back();
		batch.element.material = Material::load_material("Materials" + PATH_SEP + "Batch.mat", false, std::to_string(batch_count++));
		batch.id_transform = 0;	// use identity unless modified later ( eg. animations)

		if (usage == GL_STATIC_DRAW)
			batch.tag = RENDER_STATIC;
		else if (usage == GL_DYNAMIC_DRAW)
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

		if (usage == GL_STATIC_DRAW)
		{
			batch.element.vbo.Generate(&buffer[0], buffer.size() * sizeof(Vertex), GL_STATIC_DRAW);
		}
		else
		{
			batch.element.vbo.Generate(nullptr, sizeof(Vertex) * BatchedGraphic::max_vertices_in_batch, GL_DYNAMIC_DRAW);
		}

		batch.element.layout = layout;

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

		batch.element.vao.Unbind();
		batch.element.ibo.Unbind();


		buffer.clear();
		indices.clear();
		batch_texture_ids.clear();
		batch_textures_set.clear();
		batch_graphic_ids.clear();
		index = 0;
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

		Transform3D& transform = transforms[graphic.id_transform];
		Vector3<Float_32> p1 =  Vector3<Float_32>{ 0.5f*x_scale, 0.5f*y_scale, 0.0f };
		Vector3<Float_32> p2 =  Vector3<Float_32>{ -0.5f*x_scale, 0.5f*y_scale, 0.0f };
		Vector3<Float_32> p3 =  Vector3<Float_32>{ -0.5f*x_scale, -0.5f*y_scale, 0.0f };
		Vector3<Float_32> p4 =  Vector3<Float_32>{ 0.5f*x_scale, -0.5f*y_scale, 0.0f };

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
			float texture_index = batch_texture_ids.size() - 1;
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

				1.0f,1.0f,1.0f,1.0f,

				0.0f,
				0.0f,
				-1.0f,

				texco_u,
				texco_v,

				texture_index

			};

			Vertex v2 = {
				p2.x, p2.y, p2.z,


				1.0f,
				1.0f,
				1.0f,
				1.0f,

				0.0f,
				0.0f,
				-1.0f,

				0.f,
				texco_v,

				texture_index
			};

			Vertex v3 = {
				p3.x, p3.y, p3.z,


				1.0f,
				1.0f,
				1.0f,
				1.0f,

				0.0f,
				0.0f,
				-1.0f,

				0.0f,
				0.0f,

				texture_index
			};

			Vertex v4 = {
				p4.x, p4.y, p4.z,

				1.0f,
				1.0f,
				1.0f,
				1.0f,

				0.0f,
				0.0f,
				-1.0f,

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
		else if (usage == GL_DYNAMIC_DRAW)
		{
			batch_graphic_ids.push_back(graphic_id);
			Uint_32 id_animator = graphics[graphic_id].id_animator;
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
		
		// get texture list
		/*std::set<Uint_32> texture_ids;
		if (usage == GL_STATIC_DRAW)
		{
			for (auto i : batched_graphic_ids)
			{
				auto t = Material::get_material(graphics[i].element.material)->diffuse_textures[0];
				texture_ids.insert(t);
			}
		}
		else
		{
			for (Uint_32 i : batched_graphic_ids)
			{
				Uint_32 id_animator = graphics[i].id_animator;
				if (id_animator)
				{
					Animation& animation = animators[id_animator].animation;
					for (Keyframe& frame : animation.frames)
					{
						texture_ids.insert(frame.texture);
					}
				}
				else
				{
					auto t = Material::get_material(graphics[i].element.material)->diffuse_textures[0];
					texture_ids.insert(t);
				}

			}
		}
		std::vector<Uint_32> texture_id_vec(texture_ids.begin(), texture_ids.end());*/

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



	void RendererOpenGL2D::generate_sprite_graphics(Uint_32 graphic_id)
	{
		//Uint_32 mat_id = Material::load_material("shaders" + PATH_SEP + "DiffuseUnlit2D.shader", texture_file_path, mat_name);
		// find the proper scale needed for the quad mesh
		Graphic& graphic = graphics[graphic_id];
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
		Float_32 import_scale = Graphic::editor_data[graphic_id].scale;
		x_scale *= import_scale;
		y_scale *= import_scale;

		std::vector<GLuint> indices;
		std::vector<Vertex> buffer;

		Vertex v1={
			0.5f*x_scale,
			0.5f*y_scale,
			0.0f,

			1.0f,
			1.0f,
			1.0f,
			1.0f,

			0.0f,
			0.0f,
			-1.0f,

			1.0f,
			1.0f
		};

		Vertex v2={
			-0.5f*x_scale,
			0.5f*y_scale,
			0.f,

			1.0f,
			1.0f,
			1.0f,
			1.0f,

			0.0f,
			0.0f,
			-1.0f,
			
			0.f,
			1.0f
		};

		Vertex v3={
			-0.5f*x_scale,
			-0.5f*y_scale,
			0.f,
			
			1.0f,
			1.0f,
			1.0f,
			1.0f,
			
			0.0f,
			0.0f,
			-1.0f,
			
			0.0f,
			0.0f 
		};

		Vertex v4={
			0.5f*x_scale,
			-0.5f*y_scale,
			0.f,
			
			1.0f,
			1.0f,
			1.0f,
			1.0f,
			
			0.0f,
			0.0f,
			-1.0f,
			
			1.0f,
			0.0f
		};

        buffer.push_back(v1);
        buffer.push_back(v2);
        buffer.push_back(v3);
        buffer.push_back(v4);

		// save vertex data for collider
		Graphic::vertex_data[graphic_id][0] = Vector3<Float_32>{ v1.p_x, v1.p_y, v1.p_z };
		Graphic::vertex_data[graphic_id][1] = Vector3<Float_32>{ v2.p_x, v2.p_y, v2.p_z };
		Graphic::vertex_data[graphic_id][2] = Vector3<Float_32>{ v3.p_x, v3.p_y, v3.p_z };
		Graphic::vertex_data[graphic_id][3] = Vector3<Float_32>{ v4.p_x, v4.p_y, v4.p_z };


        indices.push_back(0);
        indices.push_back(1);
        indices.push_back(2);
        indices.push_back(2);
        indices.push_back(3);
        indices.push_back(0);

        VertexLayout layout;
        VertexAttribute attribute_0(0,3,GL_FLOAT,GL_FALSE);
        VertexAttribute attribute_1(1,4,GL_FLOAT,GL_FALSE);
        VertexAttribute attribute_2(2,3,GL_FLOAT,GL_FALSE);
        VertexAttribute attribute_3(3,2,GL_FLOAT,GL_FALSE);
        layout.add_attribute(attribute_0);
        layout.add_attribute(attribute_1);
        layout.add_attribute(attribute_2);
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


	void RendererOpenGL2D::draw_line(Vector3<Float_32> p1, Vector3<Float_32> p2, Vector4<Float_32> color)
	{
		Uint_32 mat_id = Material::load_material("Materials"+PATH_SEP+"shape.mat", true);
		//assert(mat_id);
		Material& mat = Material::material_library[mat_id];
		line_graphic.element.material = mat_id;
		Vertex v1 = { p1.x, p1.y, 0,  color.x, color.y, color.z, color.w, 0,0,0, 0,0, 0 };
		Vertex v2 = { p2.x, p2.y, 0, color.x, color.y, color.z, color.w, 0,0,0, 0,0, 0 };
		lines_buffer.push_back(v1);
		lines_buffer.push_back(v2);
		lines_indices.push_back(lines_buffer.size() - 2);
		lines_indices.push_back(lines_buffer.size() - 1);

		VertexLayout layout;
		VertexAttribute attribute_0(0, 3, GL_FLOAT, GL_FALSE);
		VertexAttribute attribute_1(1, 4, GL_FLOAT, GL_FALSE);
		VertexAttribute attribute_2(2, 3, GL_FLOAT, GL_FALSE);
		VertexAttribute attribute_3(3, 2, GL_FLOAT, GL_FALSE);
		VertexAttribute attribute_4(3, 1, GL_FLOAT, GL_FALSE);
		layout.add_attribute(attribute_0);
		layout.add_attribute(attribute_1);
		layout.add_attribute(attribute_2);
		layout.add_attribute(attribute_3);
		layout.add_attribute(attribute_4);

		line_graphic.element.vao.Generate();
		line_graphic.element.vbo.Generate(&lines_buffer[0], lines_buffer.size() * sizeof(Vertex), GL_STATIC_DRAW);
		line_graphic.element.layout = layout;

		for (auto attr : line_graphic.element.layout.vertex_attributes)
		{
			GL_CALL(
				glEnableVertexAttribArray(attr.index))
				GL_CALL(
					glVertexAttribPointer(attr.index, attr.count, attr.type, attr.normalized, layout.stride, (void*)attr.offset))
		}

		line_graphic.element.ibo.Generate(&lines_indices[0], lines_indices.size() * sizeof(GLuint), lines_indices.size());
	}

	void RendererOpenGL2D::draw_rect(Rect<Float_32> rect, Vector4<Float_32> color)
	{
		Vector3<Float_32> p1{ rect.x,rect.y,0};
		Vector3<Float_32> p2{ rect.x,rect.y+rect.h,0 };
		Vector3<Float_32> p3{ rect.x+rect.w,rect.y+rect.h, 0 };
		Vector3<Float_32> p4{ rect.x+rect.w,rect.y ,0};
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

/*
    void Renderer3D::upload_mesh(const Mesh& mesh)
    {

    }*/
}
