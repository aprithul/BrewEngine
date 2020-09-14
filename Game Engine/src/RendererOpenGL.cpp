//
//  Display.cpp
//  Game Engine
//
//  Created by Aniruddha Prithul on 7/22/17.
//  Copyright © 2017 Aniruddha Prithul. All rights reserved.
//
#include "RendererOpenGL.hpp"

namespace PrEngine {
    
    RendererOpenGL::RendererOpenGL(int width, int height, std::string title):Module("Opengl Renderer", 20)
    {
        this->width = width;
        this->height = height;
        this->title = title;
        //light_direction = 
        // initialize sdl and opengl related settings for graphics
        init();
        
        // create window
        window = SDL_CreateWindow(this->title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, this->width, this->height, SDL_WINDOW_OPENGL);
        // create the openGL context from the window  that was created
        glContext = SDL_GL_CreateContext(window);

       // GL_CALL(glFrontFace(GL_CCW))     // points are gonna get supplid in clockwise order 
       // GL_CALL(glEnable(GL_CULL_FACE))
        GL_CALL(glDisable(GL_CULL_FACE));
        //GL_CALL(glEnable(GL_CULL_FACE)); // enalbe face culling
        GL_CALL(glCullFace(GL_BACK));    // cull the back face
        GL_CALL(glFrontFace(GL_CCW));
        GL_CALL(glEnable(GL_BLEND))
        GL_CALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA))
        GL_CALL(glEnable(GL_DEPTH_TEST))
        GL_CALL(glDepthFunc(GL_LESS))
        GL_CALL(glEnable(GL_MULTISAMPLE))

        if( glewInit() != GLEW_OK)
            printf("Glew not initialized properly");

        // if context creation failed, log the error
        if(glContext==NULL)
        {
            printf("SDL_Init failed: %s\n", SDL_GetError());
        }
        else
            printf("Context created with OpenGL version  (%s)\n", glGetString(GL_VERSION));

        std::cout<<"Vector3 size"<<sizeof(Vector3<float>)<<std::endl;

        std::cout<<"Loading default texture"<<std::endl;
        Texture* _tex = new Texture(get_resource_path("default.jpg").c_str());
        texture_library[get_resource_path("default.jpg")] = _tex;
        //data = stbi_load( get_resource_path("default.jpg").c_str() ,&width, &height, &no_of_channels, 0);


        /// create render layers
        Entity* camera = entity_management_system->get_entity_with_component(COMP_CAMERA);
        long camera_handle = -1;
        if(camera == nullptr){
            LOG(LOGTYPE_ERROR, "No camera found");
            return;
        }
        else
            camera_handle = camera->id;
        std::cout<<"cam handle "<<camera_handle<<std::endl;

        GeometryLayer* geometry_layer = new GeometryLayer(camera_handle);
        render_layers.push_back(geometry_layer);

        //SpriteLayer* sprite_layer = new SpriteLayer(camera_handle);
        //render_layers.push_back(sprite_layer);

        //GuiLayer* gui_layer = new GuiLayer(window, &glContext);
        //render_layers.push_back(gui_layer);
    }
    
    
    RendererOpenGL::~RendererOpenGL()
    {
        for(std::vector<RenderLayer*>::iterator it = render_layers.begin(); it != render_layers.end(); it++)
            delete (*it);

        SDL_GL_DeleteContext(glContext);
        SDL_DestroyWindow(window);
        SDL_Quit();

    }
    
    void RendererOpenGL::init()
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

//        glEnable(GL_DEPTH_TEST); //
        //glEnable(GL_FRAMEBUFFER_SRGB);  // auto gamma correction
        
    }
    
    void RendererOpenGL::Clear(float r, float g, float b, float a)
    {
        glClearColor(r, g, b, a);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
    }
    
    void RendererOpenGL::SwapBuffers()
    {
        SDL_GL_SwapWindow(window);
    }
    
    void RendererOpenGL::set_vsync(GLboolean value)
    {
        SDL_GL_SetSwapInterval(value);
    }
    GLuint fbo;
    void RendererOpenGL::start()
    {
        //projection = Matrix4x4<float>::ortho(-1.f, 1.f, -0.75f, 0.75f,-1.f,1.f);
        //projection = Matrix4x4<float>::perspective(1.f,-1.f,2.f,1.5f);
        //projection = Matrix4x4<float>::perspective(0.f,1.f,4.f,3.f, 45.f);
        //view_matrix = Matrix4x4<float>::identity();

        //SDL_ShowCursor(0);
        //if(SDL_SetRelativeMouseMode(SDL_TRUE) == -1)
        //    LOG(LOGTYPE_ERROR, "Failed to set relative mouse mode");
        for(std::vector<RenderLayer*>::iterator it=render_layers.begin(); it!=render_layers.end(); it++)
            (*it)->start();

/*
        GLuint col_buf;
        glGenRenderbuffers(1, &col_buf);
        glBindRenderbuffer(GL_RENDERBUFFER, col_buf);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA8, width, height);
        
        GLuint dep_buf;
        glGenRenderbuffers(1, &dep_buf);
        glBindRenderbuffer(GL_RENDERBUFFER, dep_buf);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
        
        GL_CALL(glGenFramebuffers(1, &fbo))
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, col_buf);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, dep_buf);
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);
*/

    }

    bool my_tool_active = true;

    void RendererOpenGL::update()
    {
        Clear(0,0,0.3f,1);

        for(std::vector<RenderLayer*>::iterator layer=render_layers.begin(); layer!=render_layers.end(); layer++)
            (*layer)->update();
        SwapBuffers();
    }

    void RendererOpenGL::end()
    {
        for(std::vector<RenderLayer*>::iterator it=render_layers.begin(); it!=render_layers.end(); it++)
            (*it)->end();
    }

    static void CalcNormal(float N[3], float v0[3], float v1[3], float v2[3]) {
        float v10[3];
        v10[0] = v1[0] - v0[0];
        v10[1] = v1[1] - v0[1];
        v10[2] = v1[2] - v0[2];

        float v20[3];
        v20[0] = v2[0] - v0[0];
        v20[1] = v2[1] - v0[1];
        v20[2] = v2[2] - v0[2];

        N[0] = v20[1] * v10[2] - v20[2] * v10[1];
        N[1] = v20[2] * v10[0] - v20[0] * v10[2];
        N[2] = v20[0] * v10[1] - v20[1] * v10[0];

        float len2 = N[0] * N[0] + N[1] * N[1] + N[2] * N[2];
        if (len2 > 0.0f) {
            float len = sqrtf(len2);

            N[0] /= len;
            N[1] /= len;
            N[2] /= len;
        }
    }

    Sprite* RendererOpenGL::generate_graphics_sprite(const std::string& texture_file_path, const std::string& mat_name)
    {
        std::vector<GLuint> indices;
        std::vector<Vertex> buffer;

        #pragma region vertex declaration
        Vertex v1;
        v1.p_x = 0.5f;
        v1.p_y = 0.5f;
        v1.p_z = 0.0f;
        v1.c_r = 1.0f;
        v1.c_g = 1.0f;
        v1.c_b = 1.0f;
        v1.c_a = 1.0f;
        v1.n_x = 0.0f;
        v1.n_y = 0.0f;
        v1.n_z = -1.0f;
        v1.u = 1.0f;
        v1.v = 1.0f;

        Vertex v2;
        v2.p_x = -0.5f;
        v2.p_y = 0.5f;
        v2.p_z = 0.f;
        v2.c_r = 1.0f;
        v2.c_g = 1.0f;
        v2.c_b = 1.0f;
        v2.c_a = 1.0f;
        v2.n_x = 0.0f;
        v2.n_y = 0.0f;
        v2.n_z = -1.0f;
        v2.u = 0.f;
        v2.v = 1.0f;

        Vertex v3;
        v3.p_x = -0.5f;
        v3.p_y = -0.5f;
        v3.p_z = 0.f;
        v3.c_r = 1.0f;
        v3.c_g = 1.0f;
        v3.c_b = 1.0f;
        v3.c_a = 1.0f;
        v3.n_x = 0.0f;
        v3.n_y = 0.0f;
        v3.n_z = -1.0f;
        v3.u = 0.0f;
        v3.v = 0.0f;

        Vertex v4;
        v4.p_x = 0.5f;
        v4.p_y = -0.5f;
        v4.p_z = 0.f;
        v4.c_r = 1.0f;
        v4.c_g = 1.0f;
        v4.c_b = 1.0f;
        v4.c_a = 1.0f;
        v4.n_x = 0.0f;
        v4.n_y = 0.0f;
        v4.n_z = -1.0f;
        v4.u = 1.0f;
        v4.v = 0.0f;
        #pragma endregion

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



        Graphics* graphics = new Graphics();
        VertexLayout layout;
        VertexAttribute attribute_0(0,3,GL_FLOAT,GL_FALSE);
        VertexAttribute attribute_1(1,4,GL_FLOAT,GL_FALSE);
        VertexAttribute attribute_2(2,3,GL_FLOAT,GL_FALSE);
        VertexAttribute attribute_3(3,2,GL_FLOAT,GL_FALSE);
        layout.add_attribute(attribute_0);
        layout.add_attribute(attribute_1);
        layout.add_attribute(attribute_2);
        layout.add_attribute(attribute_3);

        GraphicsElement g_element;
        graphics->elements.push_back(g_element);

        std::unordered_map<std::string, Material*>::iterator _mat_it = material_library.find(mat_name);
        Material* mat;
        if(_mat_it == material_library.end())
        {
            mat = new Material("shaders"+PATH_SEP+"PassThrough.shader", std::string(texture_file_path), mat_name);
            material_library[mat_name] = mat;
        }
        else
        {
            mat = _mat_it->second;
        }
        
            graphics->elements.back().material = mat;
            graphics->elements.back().vao.Generate();
            graphics->elements.back().vbo.Generate(&buffer[0], buffer.size()*sizeof(Vertex));

            graphics->elements.back().layout = layout;
            for(std::vector<VertexAttribute>::iterator attr = graphics->elements.back().layout.vertex_attributes.begin(); attr !=  graphics->elements.back().layout.vertex_attributes.end(); attr++)
            {
                GL_CALL(
                    glEnableVertexAttribArray( attr->index))
                GL_CALL(
                    glVertexAttribPointer(attr->index, attr->count, attr->type, attr->normalized, layout.stride, (void*)attr->offset))
            }

            graphics->elements.back().ibo.Generate( &indices[0], indices.size()*sizeof(GLuint), indices.size());
            graphics->elements.back().num_of_triangles = (buffer.size()/3);

            //graphics->elements.back().material->Unbind();
            graphics->elements.back().vao.Unbind();
            graphics->elements.back().ibo.Unbind();
        
       // if(!has_transparency)
            SpriteLayer* geom_layer = (SpriteLayer*)get_layer("Sprite");
            Sprite* sprite = new Sprite(0,*graphics);
            geom_layer->sprite_list.push_back(sprite);
        
        //else
        //{
       //     SpriteLayer* tr_layer = (SpriteLayer*)get_layer("Transparency");
       //     LOG(LOGTYPE_WARNING,"after");
       //     tr_layer->graphics_list.push_back(graphics);
        //    LOG(LOGTYPE_WARNING,"after");

       // }
        
        return sprite;
    
    }

    Graphics* RendererOpenGL::generate_graphics_quad(const std::string& texture_file_path, bool has_transparency, const std::string& mat_name)
    {
        std::vector<GLuint> indices;
        std::vector<Vertex> buffer;

        #pragma region vertex declaration
        Vertex v1;
        v1.p_x = 0.5f;
        v1.p_y = 0.5f;
        v1.p_z = 0.0f;
        v1.c_r = 1.0f;
        v1.c_g = 1.0f;
        v1.c_b = 1.0f;
        v1.c_a = 1.0f;
        v1.n_x = 0.0f;
        v1.n_y = 0.0f;
        v1.n_z = -1.0f;
        v1.u = 1.0f;
        v1.v = 1.0f;

        Vertex v2;
        v2.p_x = -0.5f;
        v2.p_y = 0.5f;
        v2.p_z = 0.f;
        v2.c_r = 1.0f;
        v2.c_g = 1.0f;
        v2.c_b = 1.0f;
        v2.c_a = 1.0f;
        v2.n_x = 0.0f;
        v2.n_y = 0.0f;
        v2.n_z = -1.0f;
        v2.u = 0.f;
        v2.v = 1.0f;

        Vertex v3;
        v3.p_x = -0.5f;
        v3.p_y = -0.5f;
        v3.p_z = 0.f;
        v3.c_r = 1.0f;
        v3.c_g = 1.0f;
        v3.c_b = 1.0f;
        v3.c_a = 1.0f;
        v3.n_x = 0.0f;
        v3.n_y = 0.0f;
        v3.n_z = -1.0f;
        v3.u = 0.0f;
        v3.v = 0.0f;

        Vertex v4;
        v4.p_x = 0.5f;
        v4.p_y = -0.5f;
        v4.p_z = 0.f;
        v4.c_r = 1.0f;
        v4.c_g = 1.0f;
        v4.c_b = 1.0f;
        v4.c_a = 1.0f;
        v4.n_x = 0.0f;
        v4.n_y = 0.0f;
        v4.n_z = -1.0f;
        v4.u = 1.0f;
        v4.v = 0.0f;
        #pragma endregion

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



        Graphics* graphics = new Graphics();
        VertexLayout layout;
        VertexAttribute attribute_0(0,3,GL_FLOAT,GL_FALSE);
        VertexAttribute attribute_1(1,4,GL_FLOAT,GL_FALSE);
        VertexAttribute attribute_2(2,3,GL_FLOAT,GL_FALSE);
        VertexAttribute attribute_3(3,2,GL_FLOAT,GL_FALSE);
        layout.add_attribute(attribute_0);
        layout.add_attribute(attribute_1);
        layout.add_attribute(attribute_2);
        layout.add_attribute(attribute_3);

        GraphicsElement g_element;
        graphics->elements.push_back(g_element);
        std::unordered_map<std::string, Material*>::iterator _mat_it = material_library.find(mat_name);
        Material* mat;
        if(_mat_it == material_library.end())
        {
            mat = new Material("shaders"+PATH_SEP+"PassThrough.shader", std::string(texture_file_path), mat_name);
            material_library[mat_name] = mat;
        }
        else
        {
            mat = _mat_it->second;
        }
        
        graphics->elements.back().material = mat;
        graphics->elements.back().vao.Generate();
        graphics->elements.back().vbo.Generate(&buffer[0], buffer.size()*sizeof(Vertex));

        graphics->elements.back().layout = layout;
        for(std::vector<VertexAttribute>::iterator attr = graphics->elements.back().layout.vertex_attributes.begin(); attr !=  graphics->elements.back().layout.vertex_attributes.end(); attr++)
        {
            GL_CALL(
                glEnableVertexAttribArray( attr->index))
            GL_CALL(
                glVertexAttribPointer(attr->index, attr->count, attr->type, attr->normalized, layout.stride, (void*)attr->offset))
        }

        graphics->elements.back().ibo.Generate( &indices[0], indices.size()*sizeof(GLuint), indices.size());
        graphics->elements.back().num_of_triangles = (buffer.size()/3);

        //graphics->elements.back().material->Unbind();
        graphics->elements.back().vao.Unbind();
        graphics->elements.back().ibo.Unbind();
        
       // if(!has_transparency)
        {
            GeometryLayer* geom_layer = (GeometryLayer*)get_layer("Geometry");
            geom_layer->graphics_list.push_back(graphics);
        }
        //else
        //{
       //     SpriteLayer* tr_layer = (SpriteLayer*)get_layer("Transparency");
       //     LOG(LOGTYPE_WARNING,"after");
       //     tr_layer->graphics_list.push_back(graphics);
        //    LOG(LOGTYPE_WARNING,"after");

       // }
        
        return graphics;
    
    }

    Graphics* RendererOpenGL::generate_graphics_skybox(const std::vector<std::string>& texture_file_path, const std::string& mat_name)
    {
        std::vector<GLuint> indices;
        std::vector<Vertex> buffer;

        #pragma region vertex declaration
        Vertex v1(0.5f,0.5f,0.5f,0.f,0.f,  1.f);
        Vertex v2( 0.5f,-0.5f,0.5f,0.f,0.f, 1.f);
        Vertex v3(-0.5f,-0.5f,0.5f,0.f,0.f, 1.f);
        Vertex v4(-0.5f,0.5f,0.5f,0.f,0.f, 1.f);

        Vertex v5(0.5f,0.5f,-0.5f,0.f,0.f,  -1.f);
        Vertex v6(-0.5f,0.5f,-0.5f,0.f,0.f, -1.f);
        Vertex v7(-0.5f,-0.5f,-0.5f,0.f,0.f, -1.f);
        Vertex v8( 0.5f,-0.5f,-0.5f,0.f,0.f, -1.f);
        
        Vertex v9(  0.5f,0.5f,-0.5f,0.f,1.f, 0.f);
        Vertex v10( 0.5f,0.5f,0.5f,0.f, 1.f,  0.f);
        Vertex v11(-0.5f,0.5f,0.5f,0.f, 1.f,  0.f);
        Vertex v12(-0.5f,0.5f,-0.5f,0.f,1.f,  0.f);
        
        Vertex v13(0.5f,-0.5f,-0.5f,0.f, -1.f,   0.f);
        Vertex v14(-0.5f,-0.5f,-0.5f,0.f,-1.f,  0.f);
        Vertex v15(-0.5f,-0.5f,0.5f,0.f, -1.f,  0.f);
        Vertex v16( 0.5f,-0.5f,0.5f,0.f, -1.f,  0.f);

        Vertex v17(0.5f,0.5f,0.5f,    1.f,0.f,  0.f);
        Vertex v18(0.5f,0.5f,-0.5f,   1.f,0.f,  0.f);
        Vertex v19(0.5f,-0.5f,-0.5f,  1.f,0.f,  0.f);
        Vertex v20(0.5f,-0.5f,0.5f,   1.f,0.f,  0.f);
 
        Vertex v21(-0.5f,0.5f,-0.5f,   -1.f,0.f,  0.f);
        Vertex v22(-0.5f,0.5f, 0.5f,   -1.f,0.f,  0.f);
        Vertex v23(-0.5f,-0.5f,0.5f,   -1.f,0.f,  0.f);
        Vertex v24(-0.5f,-0.5f,-0.5f,  -1.f,0.f,  0.f);
 

        #pragma endregion

        buffer.push_back(v1);
        buffer.push_back(v2);
        buffer.push_back(v3);
        buffer.push_back(v4);
        buffer.push_back(v5);
        buffer.push_back(v6);
        buffer.push_back(v7);
        buffer.push_back(v8);       
        buffer.push_back(v9);
        buffer.push_back(v10);
        buffer.push_back(v11);      
        buffer.push_back(v12);
        buffer.push_back(v13);
        buffer.push_back(v14);       
        buffer.push_back(v15);
        buffer.push_back(v16);
        buffer.push_back(v17);       
        buffer.push_back(v18);
        buffer.push_back(v19);
        buffer.push_back(v20);       
        buffer.push_back(v21);
        buffer.push_back(v22);
        buffer.push_back(v23);
        buffer.push_back(v24);

        indices.push_back(0);
        indices.push_back(1);
        indices.push_back(2);
        indices.push_back(2);
        indices.push_back(3);
        indices.push_back(0);

        indices.push_back(4);
        indices.push_back(5);
        indices.push_back(6);
        indices.push_back(6);
        indices.push_back(7);
        indices.push_back(4);

        indices.push_back(8);
        indices.push_back(9);
        indices.push_back(10);
        indices.push_back(10);
        indices.push_back(11);
        indices.push_back(8);

        indices.push_back(12);
        indices.push_back(13);
        indices.push_back(14);
        indices.push_back(14);
        indices.push_back(15);
        indices.push_back(12);

        indices.push_back(16);
        indices.push_back(17);
        indices.push_back(18);
        indices.push_back(18);
        indices.push_back(19);
        indices.push_back(16);

        indices.push_back(20);
        indices.push_back(21);
        indices.push_back(22);
        indices.push_back(22);
        indices.push_back(23);
        indices.push_back(20);




        Graphics* graphics = new Graphics();
        VertexLayout layout;
        VertexAttribute attribute_0(0,3,GL_FLOAT,GL_FALSE);
        VertexAttribute attribute_1(1,4,GL_FLOAT,GL_FALSE);
        VertexAttribute attribute_2(2,3,GL_FLOAT,GL_FALSE);
        VertexAttribute attribute_3(3,2,GL_FLOAT,GL_FALSE);
        layout.add_attribute(attribute_0);
        layout.add_attribute(attribute_1);
        layout.add_attribute(attribute_2);
        layout.add_attribute(attribute_3);

        GraphicsElement g_element;
        graphics->elements.push_back(g_element);
        std::unordered_map<std::string, Material*>::iterator _mat_it = material_library.find(mat_name);
        Material* mat;
        if(_mat_it == material_library.end())
        {
            mat = new Material("shaders"+PATH_SEP+"SkyBox.shader", texture_file_path, mat_name);
            material_library[mat_name] = mat;
        }
        else
        {
            mat = _mat_it->second;
        }
        


        graphics->elements.back().material = mat;
        graphics->elements.back().vao.Generate();
        graphics->elements.back().vbo.Generate(&buffer[0], buffer.size()*sizeof(Vertex));

        graphics->elements.back().layout = layout;
        for(std::vector<VertexAttribute>::iterator attr = graphics->elements.back().layout.vertex_attributes.begin(); attr !=  graphics->elements.back().layout.vertex_attributes.end(); attr++)
        {
            GL_CALL(
                glEnableVertexAttribArray( attr->index))
            GL_CALL(
                glVertexAttribPointer(attr->index, attr->count, attr->type, attr->normalized, layout.stride, (void*)attr->offset))
        }

        graphics->elements.back().ibo.Generate( &indices[0], indices.size()*sizeof(GLuint), indices.size());
        graphics->elements.back().num_of_triangles = (buffer.size()/3);

        //graphics->elements.back().material->Unbind();
        graphics->elements.back().vao.Unbind();
        graphics->elements.back().ibo.Unbind();

        
        
       // if(!has_transparency)
        {
            GeometryLayer* geom_layer = (GeometryLayer*)get_layer("Geometry");
            geom_layer->graphics_list.push_back(graphics);
        }
        //else
        //{
       //     SpriteLayer* tr_layer = (SpriteLayer*)get_layer("Transparency");
       //     LOG(LOGTYPE_WARNING,"after");
       //     tr_layer->graphics_list.push_back(graphics);
        //    LOG(LOGTYPE_WARNING,"after");

       // }
        
        return graphics;
    
    }

    Graphics* RendererOpenGL::generate_graphics(const std::string& base_dir, const std::string& file_name, const std::string& texture_file_path, const std::string& mat_name, TextureCubeMap* env_map)
    {
        tinyobj::attrib_t attrib;
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;

        std::string warn;
        std::string err;
        bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, file_name.c_str());//, base_dir.c_str());
        if (!warn.empty())
            LOG(LOGTYPE_WARNING, warn);
        if (!err.empty())
            LOG(LOGTYPE_ERROR, err);
        if (!ret) {
            LOG(LOGTYPE_ERROR,"Loading ", std::string(file_name)," failed");
            return nullptr;
        }   
        else
            LOG(LOGTYPE_GENERAL, "Successfully loaded mesh ", std::string(file_name));

        Graphics* graphics = new Graphics();                
        VertexLayout layout;
        VertexAttribute attribute_0(0,3,GL_FLOAT,GL_FALSE);
        VertexAttribute attribute_1(1,4,GL_FLOAT,GL_FALSE);
        VertexAttribute attribute_2(2,3,GL_FLOAT,GL_FALSE);
        VertexAttribute attribute_3(3,2,GL_FLOAT,GL_FALSE);
        layout.add_attribute(attribute_0);
        layout.add_attribute(attribute_1);
        layout.add_attribute(attribute_2);
        layout.add_attribute(attribute_3);
        /*
        std::cout<<"ind size: "<<shapes[0].mesh.indices.size()<<std::endl;
        std::cout<<"vertices size: "<<attrib.vertices.size()<<std::endl;
        for( int i=0; i<shapes[0].mesh.indices.size(); i++)
        {
            std::cout<<"ind: "<<shapes[0].mesh.indices[i].vertex_index<<std::endl;
            std::cout<<" v1: "<< attrib.vertices[shapes[0].mesh.indices[i].vertex_index + 0];
            std::cout<<" v2: "<< attrib.vertices[shapes[0].mesh.indices[i].vertex_index + 1];
            std::cout<<" v3: "<< attrib.vertices[shapes[0].mesh.indices[i].vertex_index + 2]<<std::endl;
            
        }*/

        float bmin[3], bmax[3];
        for (size_t s = 0; s < shapes.size(); s++) 
        {
            std::vector<GLuint> indices;
            std::vector<Vertex> buffer;
            /*for (size_t f = 0; f < shapes[s].mesh.indices.size() / 3; f++) 
            {
                tinyobj::index_t idx0 = shapes[s].mesh.indices[3 * f + 0];
                tinyobj::index_t idx1 = shapes[s].mesh.indices[3 * f + 1];
                tinyobj::index_t idx2 = shapes[s].mesh.indices[3 * f + 2];

                float tc[3][2];
                if (attrib.texcoords.size() > 0) 
                {
                    if ((idx0.texcoord_index < 0) || (idx1.texcoord_index < 0) ||
                        (idx2.texcoord_index < 0)) 
                    {
                        // face does not contain valid uv index.
                        tc[0][0] = 0.0f;
                        tc[0][1] = 0.0f;
                        tc[1][0] = 0.0f;
                        tc[1][1] = 0.0f;
                        tc[2][0] = 0.0f;
                        tc[2][1] = 0.0f;
                    } 
                    else 
                    {
                        assert(attrib.texcoords.size() >
                            size_t(2 * idx0.texcoord_index + 1));
                        assert(attrib.texcoords.size() >
                            size_t(2 * idx1.texcoord_index + 1));
                        assert(attrib.texcoords.size() >
                            size_t(2 * idx2.texcoord_index + 1));

                        // Flip Y coord.
                        tc[0][0] = attrib.texcoords[2 * idx0.texcoord_index];
                        tc[0][1] = attrib.texcoords[2 * idx0.texcoord_index + 1];
                        tc[1][0] = attrib.texcoords[2 * idx1.texcoord_index];
                        tc[1][1] = attrib.texcoords[2 * idx1.texcoord_index + 1];
                        tc[2][0] = attrib.texcoords[2 * idx2.texcoord_index];
                        tc[2][1] = attrib.texcoords[2 * idx2.texcoord_index + 1];
                    }
                } 
                else 
                {
                    tc[0][0] = 0.0f;
                    tc[0][1] = 0.0f;
                    tc[1][0] = 0.0f;
                    tc[1][1] = 0.0f;
                    tc[2][0] = 0.0f;
                    tc[2][1] = 0.0f;
                }

                float v[3][3];
                for (int k = 0; k < 3; k++) 
                {
                    int f0 = idx0.vertex_index;
                    int f1 = idx1.vertex_index;
                    int f2 = idx2.vertex_index;
                    assert(f0 >= 0);
                    assert(f1 >= 0);
                    assert(f2 >= 0);

                    v[0][k] = attrib.vertices[3 * f0 + k];
                    v[1][k] = attrib.vertices[3 * f1 + k];
                    v[2][k] = attrib.vertices[3 * f2 + k];
                    bmin[k] = std::min(v[0][k], bmin[k]);
                    bmin[k] = std::min(v[1][k], bmin[k]);
                    bmin[k] = std::min(v[2][k], bmin[k]);
                    bmax[k] = std::max(v[0][k], bmax[k]);
                    bmax[k] = std::max(v[1][k], bmax[k]);
                    bmax[k] = std::max(v[2][k], bmax[k]);
                }

                float n[3][3];
                {
                    bool invalid_normal_index = false;
                    if (attrib.normals.size() > 0) 
                    {
                        int nf0 = idx0.normal_index;
                        int nf1 = idx1.normal_index;
                        int nf2 = idx2.normal_index;

                        if ((nf0 < 0) || (nf1 < 0) || (nf2 < 0)) 
                        {
                            // normal index is missing from this face.
                            invalid_normal_index = true;
                        } 
                        else
                        {
                            for (int k = 0; k < 3; k++) 
                            {
                                assert(size_t(3 * nf0 + k) < attrib.normals.size());
                                assert(size_t(3 * nf1 + k) < attrib.normals.size());
                                assert(size_t(3 * nf2 + k) < attrib.normals.size());
                                n[0][k] = attrib.normals[3 * nf0 + k];
                                n[1][k] = attrib.normals[3 * nf1 + k];
                                n[2][k] = attrib.normals[3 * nf2 + k];
                            }
                        }
                    } 
                    else 
                    {
                        invalid_normal_index = true;
                    }

                    if (invalid_normal_index) 
                    {
                        // compute geometric normal
                        CalcNormal(n[0], v[0], v[1], v[2]);
                        n[1][0] = n[0][0];
                        n[1][1] = n[0][1];
                        n[1][2] = n[0][2];
                        n[2][0] = n[0][0];
                        n[2][1] = n[0][1];
                        n[2][2] = n[0][2];
                    }
                }

                for (int k = 0; k < 3; k++) 
                {
                    Vertex vert;
                    vert.p_x = (v[k][0]);
                    vert.p_y = (v[k][1]);
                    vert.p_z = (v[k][2]);
                    vert.n_x = (n[k][0]);
                    vert.n_y = (n[k][1]);
                    vert.n_z = (n[k][2]);
                    // Combine normal and diffuse to get color.
                    float normal_factor = 0.2;
                    float diffuse_factor = 1 - normal_factor;
                    float c[3] = {1.f,1.f,1.f};
                    float len2 = c[0] * c[0] + c[1] * c[1] + c[2] * c[2];
                    if (len2 > 0.0f) 
                    {
                        float len = sqrtf(len2);
                        c[0] /= len;
                        c[1] /= len;
                        c[2] /= len;
                    }

                    vert.c_r = (c[0] * 0.5 + 0.5);
                    vert.c_g = (c[1] * 0.5 + 0.5);
                    vert.c_b = (c[2] * 0.5 + 0.5);
                    vert.c_a = 1.f;

                    vert.u = (tc[k][0]);
                    vert.v = (tc[k][1]);
                    buffer.push_back(vert);
                }
            }
*/


            


            std::unordered_map<std::string, GLuint> indices_map;
            GLuint index = 0;
            std::cout<<"2: "<<shapes.size()<<std::endl;
                
            for(int i=0; i< shapes[s].mesh.indices.size(); i++)
            {
                std::string key =   
                            std::to_string(shapes[s].mesh.indices[i].normal_index).append(
                            std::to_string(shapes[s].mesh.indices[i].texcoord_index)).append(
                            std::to_string(shapes[s].mesh.indices[i].vertex_index));

                std::unordered_map<std::string, GLuint>::iterator item;
                if( (item = indices_map.find(key)) != indices_map.end() )
                {

                    tinyobj::index_t ind = shapes[s].mesh.indices[i];
                    //std::cout<<attrib.vertices[ind.vertex_index+0]<<","<<attrib.vertices[ind.vertex_index+1]<<","<<attrib.vertices[ind.vertex_index+2]<<std::endl;
                    indices.push_back(item->second);
                }
                else
                {

                    tinyobj::index_t ind = shapes[s].mesh.indices[i];
                    
                    Vertex vert;
                    vert.p_x = attrib.vertices[(ind.vertex_index*3)+0];
                    vert.p_y = attrib.vertices[(ind.vertex_index*3)+1];
                    vert.p_z = attrib.vertices[(ind.vertex_index*3)+2];
                    vert.c_r = 1.f;
                    vert.c_g = 1.f;
                    vert.c_b = 1.f;
                    vert.c_a = 1.f;
                    vert.n_x = attrib.normals[ (ind.normal_index*3) + 0];
                    vert.n_y = attrib.normals[ (ind.normal_index*3) + 1];
                    vert.n_z = attrib.normals[ (ind.normal_index*3) + 2];
                    vert.u   = attrib.texcoords[ (ind.texcoord_index*2) +0];
                    vert.v   = attrib.texcoords[ (ind.texcoord_index*2) +1];
                    buffer.push_back(vert);

                    indices_map[key] = index;
                    //std::cout<<ind.vertex_index<<"//"<<ind.normal_index<<<<":"<<attrib.vertices[ind.vertex_index+0]<<","<<attrib.vertices[ind.vertex_index+1]<<","<<attrib.vertices[ind.vertex_index+2]<<std::endl;

                    indices.push_back(index);
                    index++;
                }
            }   

            std::cout<<"Buffer: "<<buffer.size()<<std::endl;
            std::cout<<"Indices: "<<indices.size()<<std::endl;


            /*for(int i=0; i<indices.size(); i++)
                std::cout<<indices[i]<<std::endl;
            for(int i=0; i<buffer.size(); i++)
                std::cout<<buffer[i].p_x<<","<<buffer[i].p_y<<","<<buffer[i].p_z<<std::endl;*/
            //std::cout<<indices[i]<<" : "<<buffer[i].p_x<<","<<buffer[i].p_y<<","<<buffer[i].p_z<<std::endl;

            //IndexBuffer ib( &(shapes[s].mesh.indices[0]), )
            //for(int i=0; i<buffer.size(); i++)
            //    std::cout<< ((&buffer[0])+i)->p_x<<","<<((&buffer[0])+i)->p_y<<","<<((&buffer[0])+i)->p_z <<std::endl;

            GraphicsElement g_element;

            graphics->elements.push_back(g_element);
            std::unordered_map<std::string, Material*>::iterator _mat_it = material_library.find(mat_name);
            Material* mat;
            if(_mat_it == material_library.end())
            {
                
                if(env_map == nullptr)
                    mat = new Material("shaders"+PATH_SEP+"Diffuse.shader", std::string(texture_file_path), mat_name);
                else
                    mat = new Material("shaders"+PATH_SEP+"Reflective.shader", std::string(texture_file_path), *env_map, mat_name);

                material_library[mat_name] = mat;
            }
            else
            {
                mat = material_library[mat_name];//_mat_it->second;
            }
            
            graphics->elements.back().material = mat;
            graphics->elements.back().vao.Generate();
            graphics->elements.back().vbo.Generate(&buffer[0], buffer.size()*sizeof(Vertex));

            graphics->elements.back().layout = layout;
            for(std::vector<VertexAttribute>::iterator attr = graphics->elements.back().layout.vertex_attributes.begin(); attr !=  graphics->elements.back().layout.vertex_attributes.end(); attr++)
            {
                GL_CALL(
                    glEnableVertexAttribArray( attr->index))
                GL_CALL(
                    glVertexAttribPointer(attr->index, attr->count, attr->type, attr->normalized, layout.stride, (void*)attr->offset))
            }

            graphics->elements.back().ibo.Generate( &indices[0], indices.size()*sizeof(GLuint), indices.size());
            graphics->elements.back().num_of_triangles = (buffer.size()/3);

            //graphics->elements.back().material->Unbind();
            graphics->elements.back().vao.Unbind();
            graphics->elements.back().ibo.Unbind();
        }
        /*
        for(int i=0; i<graphics->elements.size(); i++)
        {
            graphics->elements[i].vao.Bind();
            graphics->elements[i].vbo.Bind();
            for(std::vector<VertexAttribute>::iterator attr = graphics->elements[i].layout.vertex_attributes.begin(); attr !=  graphics->elements[i].layout.vertex_attributes.end(); attr++)
            {
                GL_CALL(
                    glEnableVertexAttribArray( attr->index))
                GL_CALL(
                    glVertexAttribPointer(attr->index, attr->count, attr->type, attr->normalized, layout.stride, (void*)attr->offset))
            }
            graphics->elements[i].vbo.Unbind();
            graphics->elements[i].vao.Unbind();
        }*/
        /*for(std::vector<RenderLayer*>::iterator it=render_layers.begin(); it!=render_layers.end(); it++)
        {
                if((*it)->name == "Geometry"){
                    ((GeometryLayer*)(*it))->graphics3d_list.push_back(graphics);
                    std::cout<<"added graphics"<<std::endl;
                }
                std::cout<<"name : "<<(*it)->name<<std::endl;
        }*/

        GeometryLayer* geom_layer = (GeometryLayer*)get_layer("Geometry");

        if(geom_layer != nullptr)
            geom_layer->graphics_list.push_back(graphics);

        std::cout<<"returning"<<std::endl;
        return graphics;

    }

    RenderLayer* RendererOpenGL::get_layer(const std::string& layer_name)
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