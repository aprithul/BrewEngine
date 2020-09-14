//
//  Display.h
//  Game Engine
//
//  Created by Aniruddha Prithul on 7/22/17.
//  Copyright © 2017 Aniruddha Prithul. All rights reserved.
//

#ifndef Display_h
#define Display_h

#include "tiny_obj_loader.h"
#include "GlAssert.hpp"

#include "Material.hpp"
#include "Graphics.hpp"
#include "Module.hpp"
#include "RenderLayer.hpp"
#include "GeometryLayer.hpp"
#include "SpriteLayer.hpp"
#include "GuiLayer.hpp"
#include <vector>

namespace PrEngine {
    
    class RendererOpenGL : public Module
    {

    public:
        std::vector<RenderLayer*> render_layers;

        // display attributes
        int height;
        int width;
        std::string title;
        Vector3<float> light_direction;

        // constructor/ destructors
        RendererOpenGL(int width, int height, std::string title);
        ~RendererOpenGL();
        void init();
        // clears the display with the color specified
        void Clear(float r, float g, float b, float a);
        // swaps buffers in a double buffer environment
        void SwapBuffers();
        void set_vsync(GLboolean value);
        bool make_shader_program(const std::string& path, GLuint& shader_program);
        GLuint make_shader( GLenum type, const std::string& source);
        Graphics* generate_graphics(const std::string& base_dir, const std::string& file_name, const std::string& texture_file_path, const std::string& mat_name, TextureCubeMap* env_map = nullptr);
        Graphics* generate_graphics_quad(const std::string& texture_file_path, bool has_transparency, const std::string& mat_name);
        Graphics* generate_graphics_skybox( const std::vector<std::string>& texture_file_path, const std::string& mat_name);
        Sprite* generate_graphics_sprite(const std::string& texture_file_path, const std::string& mat_name);
        RenderLayer* get_layer(const std::string& layer_name);
        //Matrix4x4<float> view_matrix;
        //Matrix4x4<float> projection;

        void start() override;
        void update() override;
        void end() override;
        // reference to the sdl window
        SDL_Window* window;
        // the opengl context used with the window
        SDL_GLContext glContext;
    };
}

#endif /* Display_h */