//
//  Display.cpp
//  Game Engine
//
//  Created by Aniruddha Prithul on 7/22/17.
//  Copyright © 2017 Aniruddha Prithul. All rights reserved.
//
#include "RendererOpenGL2D.hpp"

namespace PrEngine {

	RendererOpenGL2D* renderer = nullptr;

	RendererOpenGL2D::RendererOpenGL2D(Int_32 width, Int_32 height, Bool_8 full_screen, std::string& title, std::string module_name, Int_32 priority):Module(module_name, priority)
    {
		assert(renderer == nullptr);

        this->width = width;
        this->height = height;
        this->title = title;
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
		Uint_32 mat = Material::load_material("Materials\\Default.mat");
		
		assert(Material::material_creation_status); // default material has to be created for engine to work

        //data = stbi_load( get_resource_path("default.jpg").c_str() ,&width, &height, &no_of_channels, 0);


        /// create render layers
        //GeometryLayer* geometry_layer = new GeometryLayer(camera_handle);
        //render_layers.push_back(geometry_layer);

        SpriteLayer* sprite_layer = new SpriteLayer();
        render_layers.push_back(sprite_layer);

        GuiLayer* gui_layer = new GuiLayer(window, &glContext);
        render_layers.push_back(gui_layer);

		renderer = this;

		GLint texture_units;
		glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &texture_units);
		max_textures_in_batch = (Uint_32)texture_units;
		LOG(LOGTYPE_WARNING, "texture units: "+std::to_string(max_textures_in_batch));
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

	void RendererOpenGL2D::generate_batched_sprite_graphics(Uint_32 graphic_id)
	{

	}


	void RendererOpenGL2D::generate_sprite_graphics(Uint_32 graphic_id)
	{
		//Uint_32 mat_id = Material::load_material("shaders" + PATH_SEP + "DiffuseUnlit2D.shader", texture_file_path, mat_name);
		// find the proper scale needed for the quad mesh
		Graphic& graphic = graphics[graphic_id];
		Uint_32 material_id = graphic.element.material;
		assert(material_id);

		Material* mat = Material::get_material(material_id);
		Texture* tex = Texture::get_texture(mat->diffuse_texture); //Material::material_library[mat_id].diffuse_texture;
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

		Vertex v4{
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
		graphic.element.vbo.Generate(&buffer[0], buffer.size() * sizeof(Vertex));
		graphic.element.layout = layout;
		for (std::vector<VertexAttribute>::iterator attr = graphic.element.layout.vertex_attributes.begin(); attr != graphic.element.layout.vertex_attributes.end(); attr++)
		{
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
		Uint_32 mat_id = Material::load_material("default.mat");
		assert(mat_id);
		Material& mat = Material::material_library[mat_id];
		line_graphic.element.material = mat_id;
		Vertex v1 = { p1.x, p1.y, 0,0,0,0, color.x, color.y, color.z, color.w };
		Vertex v2 = { p2.x, p2.y, 0,0,0,0, color.x, color.y, color.z, color.w };
		lines_buffer.push_back(v1);
		lines_buffer.push_back(v2);
		lines_indices.push_back(lines_buffer.size() - 2);
		lines_indices.push_back(lines_buffer.size() - 1);

		VertexLayout layout;
		VertexAttribute attribute_0(0, 3, GL_FLOAT, GL_FALSE);
		VertexAttribute attribute_1(1, 4, GL_FLOAT, GL_FALSE);
		VertexAttribute attribute_2(2, 3, GL_FLOAT, GL_FALSE);
		layout.add_attribute(attribute_0);
		layout.add_attribute(attribute_1);
		layout.add_attribute(attribute_2);

		line_graphic.element.vao.Generate();
		line_graphic.element.vbo.Generate(&lines_buffer[0], lines_buffer.size() * sizeof(Vertex));
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
