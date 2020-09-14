//
//  Display.h
//  Game Engine
//
//  Created by Aniruddha Prithul on 7/22/17.
//  Copyright © 2017 Aniruddha Prithul. All rights reserved.
//


#ifndef RENDERCOMPONENET_HPP
#define RENDERCOMPONENET_HPP

#define MAX_RENDERED_GRAPHICS_PER_FRAME 999999


#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <queue>
#include <utility>
#include "Engine.hpp"
#include "Module.hpp"
#include "TimeModule.hpp"
#include "Logger.hpp"
#include "Transform.hpp"
#include "Component.hpp"

namespace PrEngine {
    class Graphics;
    
    // globally required functions that are related to renderer but shouldn't require access to the renderer
    extern SDL_Texture* load_texture(const std::string &file, SDL_Renderer *ren); 
    extern std::map<std::string, SDL_Texture*> loaded_texture_library;
    //////////////////////////////////////////////////////////////////////////////////////////////////////

    struct GraphicsFrame
    {
        SDL_Texture* texture;
        SDL_Rect region;
    };

    struct TextureSlicingParameters
    {
        int x;
        int y;
        int w;
        int h;
        int x_pad;
        int y_pad;

        TextureSlicingParameters(int x, int y, int w, int h, int x_pad, int y_pad)
        {
            this->x = x;
            this->y = y;
            this->w = w;
            this->h = h;
            this->x_pad = x_pad;
            this->y_pad = y_pad;
        }
        TextureSlicingParameters(int x, int y, int dim, int pad)
        {
            this->x = x;
            this->y = y;
            this->w = dim;
            this->h = dim;
            this->x_pad = pad;
            this->y_pad = pad;
        }

        

    };

    enum TextJustification
    {
        TOP_LEFT,
        TOP_CENTER,
        TOP_RIGHT,
        MIDDLE_LEFT,
        MIDDLE_CENTER,
        MIDDLE_RIGHT,
        BOTTOM_LEFT,
        BOTTOM_CENTER,
        BOTTOM_RIGHT
    };

    class Renderer2D : public Module
    {
    public:

        // reference to the sdl window
        SDL_Window* sdl_window = NULL;
        SDL_Renderer* sdl_renderer = NULL;
        SDL_Texture* render_texture = NULL;
        // display attributes
        int window_height;
        int window_width;
        int world_unit_to_pixels;
        float zoom_amount;
        Vector2<float> view_position;

        Renderer2D(int width, int height, std::string title, bool vsync, int world_unit_to_pixels, std::string name, int priority);
        Renderer2D();
        ~Renderer2D();
        
        void set_frame_rate(int frame_rate);
        void set_draw_color(SDL_Color color);
        int add_graphics_to_draw(Graphics* graphics);
        void remove_graphics(int id);
        SDL_Rect get_world_to_screen_rect(SDL_FRect rect);

        void draw_rectangle(SDL_FRect rect, SDL_Color color, bool screen_space = false, bool centered = false);
        void draw_rectangle(SDL_Rect rect, SDL_Color color, bool screen_space = false, bool centered = false);
        void draw_line(Vector2<float> p1, Vector2<float>p2, SDL_Color color, bool screen_space = false);
        TTF_Font* open_font(const std::string font_file_name,int font_size);
        SDL_Texture* draw_text_debug(const std::string& text, TTF_Font* font, SDL_Color& color,Vector2<int> screen_position, TextJustification text_justification);
        SDL_Texture* get_text_texture(const std::string& text, TTF_Font* font, SDL_Color& color);
        void draw_text(SDL_Texture* texture, SDL_Rect* dst_region, TextJustification text_justification, int height = -1);
        void draw_text(SDL_Texture* texture, Vector2<int> screen_position, TextJustification text_justification, int height = -1);
        void draw_text(const std::string& text, TTF_Font* font, SDL_Color& color,Vector2<int> screen_position, TextJustification text_justification);
        void close_font(TTF_Font* font);
        void sort_render_order();

        void start() override;
        void update() override;
        void end() override;
        
    private:
        
        std::string title;
        SDL_Color clear_color;
        Graphics* render_list[MAX_RENDERED_GRAPHICS_PER_FRAME];
        std::queue<int> released_positions;
        std::queue<std::pair<SDL_Color,SDL_Rect> > debug_shapes_rect;
        std::queue<std::pair<SDL_Color, std::pair<Vector2<float>,Vector2<float> > > > debug_shapes_line;
        std::queue<std::pair<SDL_Texture*, SDL_Rect> > ttf_textures;
        void justify_text(TextJustification tj, SDL_Rect& rect);
        int render_array_head;
        bool do_draw_debug_shapes;

        // draws 'everything' on screen inside renderer update
        void draw();
        void draw_debug();
        void draw_ttf_textures();
        void set_vsync(bool value);
        
    };



    class Graphics : public Component
    {
    private:
        
        GraphicsFrame* graphics_frames;  //we want to be able to store multiple animation frames
        
    public:
        int layer;
        SDL_Rect dst_dimension;
        float angle;
        int current_frame_index;
        int number_of_frames;
        Graphics();
        ~Graphics();
        GraphicsFrame* get_current_frame();
        GraphicsFrame* get_frame_at(int index);
        void draw(Renderer2D* renderer, bool centered);
        bool load_graphics(std::string graphics_file, const TextureSlicingParameters* slicing_params, const Renderer2D& renderer2d, int num_of_frames = 1, bool is_subregion = false);
        bool load_graphics(SDL_Texture* texture, const TextureSlicingParameters* slicing_params, const Renderer2D& renderer2d, int num_of_frames = 1, bool is_subregion = false);
        void add_frame_to_graphics(const GraphicsFrame& frame);
    };
}

#endif /* Display_h */