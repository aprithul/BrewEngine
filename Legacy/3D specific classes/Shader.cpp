//
//  Shader.cpp
//  Game Engine
//
//  Created by Aniruddha Prithul on 11/11/17.
//  Copyright © 2017 Aniruddha Prithul. All rights reserved.
//
#include<iostream>
#include "Shader.hpp"

namespace Pringine {

    Shader::Shader(GLenum type, const std::string& dir)
    {
        // create the shader object
        shader_object = glCreateShader( type);
       
        // load shader from file at 'dir'
        std::string source = load_source_string(dir);
        const char* _source = source.c_str();
        // load shader source to shader object and compile
        glShaderSource(shader_object, 1, &_source, NULL);
        glCompileShader(shader_object);
        
        // validate
        GLint vertex_compiled;
        glGetShaderiv(shader_object, GL_COMPILE_STATUS, &vertex_compiled);
        if (vertex_compiled != GL_TRUE)
        {
            GLsizei log_length = 0;
            GLchar message[1024];
            glGetShaderInfoLog(shader_object, 1024, &log_length, message);
            std::cout<<log_length<<std::endl;
        }
        else
        {
            std::cout<<"successfully compiled "<<(type==GL_VERTEX_SHADER?"vertex shader : ":"fragment shader : ")<<dir<<std::endl;
        }
    }
    
    std::string Shader::load_source_string(const std::string &dir)
    {
        std::ifstream ifs(dir);
        std::string content( (std::istreambuf_iterator<char>(ifs) ),
                            (std::istreambuf_iterator<char>()    ) );
        return content;
    }
    
    Shader::~Shader()
    {
        
    }
    
    GLuint Shader::get_compiled_shader_object()
    {
        return shader_object;
    }
    
}
