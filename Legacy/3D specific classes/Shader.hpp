//
//  Shader.hpp
//  Game Engine
//
//  Created by Aniruddha Prithul on 11/11/17.
//  Copyright © 2017 Aniruddha Prithul. All rights reserved.
//

#ifndef Shader_h
#define Shader_h
#include <string>
#include <OpenGL/gl3.h>
#include <fstream>
namespace Pringine {

    class Shader
    {
    public:
        Shader( GLenum type, const std::string& dir);
        ~Shader();
        GLuint get_compiled_shader_object();
        
    private:
        GLuint shader_object;
        std::string load_source_string(const std::string& dir);
    };
}


#endif /* Shader_h */
