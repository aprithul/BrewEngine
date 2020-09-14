//
//  RendererComponent.cpp
//  Game Engine
//
//  Created by Aniruddha Prithul on 7/22/17.
//  Copyright © 2017 Aniruddha Prithul. All rights reserved.

#include "RendererComponent.hpp"
namespace Pringine {
    
    RendererComponent::RendererComponent(GLint Height, GLint Width, std::string Title)
    {
        display = new Display(Height, Width, Title);
        set_clear_color(0.0f, 0.0f, 0.0f, 1.0f );
        
        // test code
        set_shaders();
        Mesh mesh = create_mesh();
        set_mesh(mesh);
    }
    
    GLvoid RendererComponent::set_shaders()
    {
        // TEST CODE
        Shader* vertexShader = new Pringine::Shader(GL_VERTEX_SHADER, "res/shaders/PassThrough.v");
        Shader* fragmentShader = new Pringine::Shader(GL_FRAGMENT_SHADER, "res/shaders/PassThrough.f");
        
        GLuint program = glCreateProgram();
        glAttachShader(program, vertexShader->get_compiled_shader_object());
        glAttachShader(program, fragmentShader->get_compiled_shader_object());
        glLinkProgram(program);
        
        // validate link
        GLint link_status;
        glGetProgramiv(program, GL_LINK_STATUS, &link_status);
        if(link_status==GL_FALSE)
        {
            GLsizei log_length = 0;
            GLchar message[1024];
            glGetProgramInfoLog(program, 1024, &log_length, message);
            std::cout<<"linking failed: "<<std::endl;
            std::cout<<message<<std::endl;
        }
        else
        {
            std::cout<<"Linking succssful"<<std::endl;
        }
        glUseProgram(program);
    }
    
    const Mesh RendererComponent::create_mesh()
    {
        Mesh mesh;
        Vertex vertices[] = {
            Vertex( -1.0f,  -1.0f,  0.0f ),
            Vertex(  0.0f,   1.0f,  0.0f ),
            Vertex(  1.0f,  -1.0f,  0.0f )
        };
        GLint mesh_vertex_count = sizeof(vertices)/sizeof(vertices[0]);
        mesh.set_vertices(vertices, mesh_vertex_count );
        return mesh;
    }
    
    GLvoid RendererComponent::set_mesh( Mesh& mesh)
    {
        // we are in core context
        // so a vertex array object is mandatory
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);
        
        // generate a vertex buffer and put data in it
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, mesh.size, mesh.get_vertex_array_flat(), GL_STATIC_DRAW);
        
        // enable position attribute, define the layout and draw
        glEnableVertexAttribArray(ATTIRB_POSITION);
        glVertexAttribPointer(ATTIRB_POSITION, Vertex::ATTRIB_POSITION_LENGTH, GL_FLOAT, GL_FALSE, Vertex::SIZE , 0);
    }

    
    RendererComponent::RendererComponent()
    {
        
    }
    
    RendererComponent::~RendererComponent()
    {
        std::cout<<"RendererComponent destructor"<<std::endl;
        delete display;
    }
    
    GLvoid RendererComponent::set_clear_color(float r, float g, float b, float a)
    {
        this->r = r;
        this->g = g;
        this->b = b;
        this->a = a;
    }
    
    GLvoid RendererComponent::set_vsync(GLboolean value)
    {
        display->set_vsync(value);
    }
    
    GLvoid RendererComponent::start()
    {
        this->accumulator = 0.0;
        this->_second = 0.0;
    }
    
    GLvoid RendererComponent::update()
    {
        if(is_active)
        {
            display->Clear(r, g, b, a);
            draw();
            display->SwapBuffers();
            this->frame_count++;
        }
    }
    
    GLvoid RendererComponent::draw()
    {
        glDrawArrays(GL_TRIANGLES, 0, 3);
    }
    
    GLvoid RendererComponent::end()
    {
        glBindBuffer(GL_ARRAY_BUFFER, 0 );
        glBindVertexArray(0);
        std::cout<<"RendererComponent Ended"<<std::endl;
        
    }

}
