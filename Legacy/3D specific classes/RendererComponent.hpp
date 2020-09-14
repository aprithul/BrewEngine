//
//  RendererComponent.hpp
//  Game Engine
//
//  Created by Aniruddha Prithul on 7/22/17.
//  Copyright © 2017 Aniruddha Prithul. All rights reserved.
//

#ifndef RendererComponent_hpp
#define RendererComponent_hpp

#include <iostream>
#include "Component.hpp"
#include "Display.hpp"
#include "TimeComponent.hpp"
#include "Mesh.hpp"
#include "Vertex.hpp"
#include "Shader.hpp"

namespace Pringine {
    
    // handles all rendering stuff
    class RendererComponent : public Component
    {
        
    public:
        
        RendererComponent(GLint Height, GLint Width, std::string Title);
        RendererComponent();
        ~RendererComponent();
        
        GLvoid set_frame_rate(GLint frame_rate);
        GLvoid set_clear_color(GLfloat r, GLfloat g, GLfloat b, GLfloat a);
        GLvoid set_vsync(GLboolean value);
        GLvoid start() override;
        GLvoid update() override;
        GLvoid end() override;
        
    private:
        // display actually implements methods to render to screen
        Display* display;
        GLfloat r,g,b,a;
        GLint frame_rate;
        GLdouble frame_delta;
        GLdouble accumulator;
        GLdouble _second;
        GLint frame_count;
        GLuint vbo;
        GLuint vao;
        GLvoid draw();
        
        // debug code
        GLvoid set_shaders();
        const Mesh create_mesh();
        GLvoid set_mesh( Mesh& mesh);
    };
}

#endif /* RendererComponent_hpp */
