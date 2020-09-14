//
//  Display.cpp
//  Game Engine
//
//  Created by Aniruddha Prithul on 7/22/17.
//  Copyright © 2017 Aniruddha Prithul. All rights reserved.
//
#include "RendererModule.hpp"

namespace PrEngine {


    // globally required functions that are related to renderer but shouldn't require access to the renderer
    std::map<std::string, SDL_Texture*> loaded_texture_library;

    SDL_Texture* load_texture(const std::string &file_name, SDL_Renderer *ren)
    {
        #if IS_SERVER
            return nullptr;
        #else
            if(loaded_texture_library.count(file_name) == 0)
            {
                if(ren == nullptr)
                {
                    LOG(LOGTYPE_ERROR, std::string("Renderer provided in load_texture is null"));
                    return nullptr;
                }

                SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY,"best");
                SDL_Texture *texture = IMG_LoadTexture(ren, file_name.c_str());
                if (texture == nullptr){
                    LOG(LOGTYPE_ERROR, std::string("Couldn't load texture : ").append(file_name));
                    return nullptr;
                }
                
                loaded_texture_library[file_name] = texture;
                LOG(LOGTYPE_GENERAL, "Loaded : ", file_name);
                return texture;
            }
            else
                LOG(LOGTYPE_GENERAL, "Pre Loaded : ", file_name);
                return loaded_texture_library[file_name];
        #endif
    }

    //////////////////////////////////////////////////////////////////////
    

    void insertion_sort(Graphics** arr, int n)  
    {  
        int i, j;  
        //LOG(LOGTYPE_GENERAL, std::to_string(n));
        Graphics* key;
        for (i = 2; i <= n; i++) 
        {  
            key = arr[i];  
            j = i - 1;  
    
            /* Move elements of arr[0..i-1], that are  
            greater than key, to one position ahead  
            of their current position */
            while (j >= 1 && arr[j]->layer > key->layer) 
            {  
                arr[j + 1] = arr[j];  
                j = j - 1;  
            }  
            arr[j + 1] = key;  
        }  

    } 

    // publics
    void Renderer2D::sort_render_order()
    {
        insertion_sort(render_list, render_array_head);
    }
    
    
    Renderer2D::Renderer2D(int width, int height, std::string title, bool vsync, int world_unit_to_pixels, std::string name, int priority):Module(name,priority)
    {
        #if !IS_SERVER
            // initialize SDL video
            SDL_Init(SDL_INIT_VIDEO);
            TTF_Init();
            
            this->window_width = width;
            this->window_height = height;
            this->title = title;
            this->zoom_amount = 1.0f;
            this->view_position.x = 0.0f;
            this->view_position.y = 0.0f;
            this->world_unit_to_pixels = world_unit_to_pixels;
            this->clear_color={255,255,255,255};
            this->do_draw_debug_shapes = true;
            // create window and sdl_renderer
            set_vsync(vsync);
            #ifndef EMSCRIPTEN
            SDL_SetHint(SDL_HINT_RENDER_BATCHING, "1");
            #endif

            sdl_window = SDL_CreateWindow(this->title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, this->window_width, this->window_height, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN);
            sdl_renderer = SDL_CreateRenderer(sdl_window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
            render_texture = SDL_CreateTexture( sdl_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, this->window_width, this->window_height );
            SDL_SetTextureBlendMode(render_texture, SDL_BLENDMODE_BLEND);

            //load_texture("default.jpg", sdl_renderer);

        #endif
    }

    void Renderer2D::set_draw_color(SDL_Color color)
    {
        #if !IS_SERVER
            SDL_SetRenderDrawColor(sdl_renderer, color.r, color.g, color.b, color.a);
        #endif
    }
    
    Renderer2D::~Renderer2D()
    {
        std::map<std::string, SDL_Texture*>::iterator ptr;
        for (ptr = loaded_texture_library.begin(); ptr != loaded_texture_library.end(); ptr++) 
        {
            if(ptr->second!=nullptr)
                SDL_DestroyTexture( ptr->second);
            else
            {
                LOG(LOGTYPE_WARNING, "Renderer found texture to be null while cleaning up");
            }
        }
        SDL_DestroyTexture(render_texture);

        TTF_Quit();
        SDL_DestroyRenderer(sdl_renderer);
        SDL_DestroyWindow(sdl_window);
    }
    
    void Renderer2D::start()
    {
        #if !IS_SERVER
            // for first frame text rendering
            SDL_SetRenderTarget(sdl_renderer, render_texture);
            set_draw_color(SDL_Color{0,0,0,0});
            SDL_RenderClear(sdl_renderer);
        #endif
    }
    
    void Renderer2D::update()
    {
        #if !IS_SERVER
        //if( PrEngine::engine->input_handler->get_key_down(SDLK_d))
        //    do_draw_debug_shapes = !do_draw_debug_shapes;

            if(is_active)
            {
                // revert back to drawing on the renderer
                SDL_SetRenderTarget(sdl_renderer, NULL);

                // start rendering
                set_draw_color(clear_color);
                SDL_RenderClear(sdl_renderer);

                draw();

                if(do_draw_debug_shapes)
                    draw_debug();
                //draw_ttf_textures();
                SDL_RenderCopy(sdl_renderer, render_texture, NULL, NULL);
                SDL_RenderPresent(sdl_renderer);
                //SDL_SetRenderTarget(sdl_renderer, render_texture);

                //reset back to render to screen
                //SDL_SetRenderTarget(sdl_renderer, NULL);
                //copy over the render texture to screen
                // before that resize the texture to apply any screen effect ( zoom / rotation)
                /*int src_w = this->window_width;// * 1/zoom_amount;
                int src_h = this->window_height;// * 1/zoom_amount;
                SDL_Rect src_rct;
                src_rct.x = (this->window_width - src_w)/2;
                src_rct.y = (this->window_height - src_h)/2;
                src_rct.w = src_w;
                src_rct.h = src_h;*/
                //SDL_RenderCopy(sdl_renderer, render_texture, &src_rct, NULL);

                // set render texture for drawing post render stuff
                SDL_SetRenderTarget(sdl_renderer, render_texture);
                set_draw_color(SDL_Color{0,0,0,0});
                SDL_RenderClear(sdl_renderer);

            }
        #endif
    }

    void Renderer2D::end()
    {
        LOG(LOGTYPE_GENERAL, "RendererComponent Ended");
    }

        
    void Renderer2D::set_vsync(bool value)
    {
        #if !IS_SERVER
            if(value == 1)
            {
                SDL_SetHint(SDL_HINT_RENDER_VSYNC, "1"); 
            }
            else
            {
                SDL_SetHint(SDL_HINT_RENDER_VSYNC, "0"); 
            }
        #endif
    }

    int Renderer2D::add_graphics_to_draw(Graphics* graphics)
    {
        #if !IS_SERVER
            if(graphics!=nullptr)
            {
                int next_pos = -1;
                if(!released_positions.empty())
                {
                    next_pos = released_positions.front();
                    released_positions.pop();
                }
                else
                {
                    if(render_array_head < MAX_RENDERED_GRAPHICS_PER_FRAME)
                    {
                        render_array_head++;
                        next_pos = render_array_head;
                    }
                    else
                    {
                        LOG(LOGTYPE_ERROR, "Cant add anymore graphics to draw, array full");
                        return -1;
                    }
                    
                }

                render_list[next_pos] = graphics;
                
                insertion_sort(render_list, render_array_head);
                return next_pos;
                
            }
            else
            {    
                LOG(LOGTYPE_ERROR, "Cant put null graphics in render_list");
                return -1;
            }
        #else
            return -1;
        #endif


    }

    void Renderer2D::remove_graphics(int id)
    {
        #if !IS_SERVER
            if(id<=render_array_head)
            {   
                Graphics* _graphics = render_list[id];    
                if(_graphics!= nullptr)
                {
                    render_list[id] = nullptr;
                    released_positions.push(id);
                }
                else
                {
                    LOG(LOGTYPE_WARNING, std::string("No graphics object with id: ").append(std::to_string(id)).append(" exists"));
                }
            }
            else
            {
                LOG(LOGTYPE_WARNING, std::string("Requested graphics object id: ").append(std::to_string(id)).append(" is out of id range"));
            }
        #endif
    }

    void Renderer2D::draw_rectangle(SDL_FRect rect, SDL_Color color, bool screen_space, bool centered)
    {
        #if !IS_SERVER
            if(!screen_space)
            {
                rect.x = ((rect.x - view_position.x) - rect.w/2.0f)*world_unit_to_pixels*zoom_amount + window_width/2.0f;
                rect.y = (-(rect.y - view_position.y) - rect.h/2.0f)*world_unit_to_pixels*zoom_amount + window_height/2.0f;
                rect.w = rect.w * world_unit_to_pixels*zoom_amount;
                rect.h = rect.h * world_unit_to_pixels*zoom_amount;
            }
            else if(centered)
            {
                rect.x -= rect.w/2.0f;
                rect.y -= rect.h/2.0f;
            }
            //debug_shapes_rect.emplace( std::make_pair(color, sdl_rect));
            SDL_SetRenderDrawColor(sdl_renderer, color.r, color.g, color. b, color.a);
            SDL_RenderDrawRectF( sdl_renderer , &rect);
        #endif
    }

    void Renderer2D::draw_rectangle(SDL_Rect rect, SDL_Color color, bool screen_space, bool centered)
    {
        #if !IS_SERVER
            if(!screen_space)
            {
                rect.x = ((rect.x - view_position.x) - rect.w/2.0f)*world_unit_to_pixels*zoom_amount + window_width/2.0f;
                rect.y = (-(rect.y - view_position.y) - rect.h/2.0f)*world_unit_to_pixels*zoom_amount + window_height/2.0f;
                rect.w = rect.w * world_unit_to_pixels*zoom_amount;
                rect.h = rect.h * world_unit_to_pixels*zoom_amount;
            }
            else if(centered)
            {
                rect.x -= rect.w/2.0f;
                rect.y -= rect.h/2.0f;
            }

            //debug_shapes_rect.emplace( std::make_pair(color, sdl_rect));
            SDL_SetRenderDrawColor(sdl_renderer, color.r, color.g, color. b, color.a);
            SDL_RenderDrawRect( sdl_renderer , &rect);
        #endif
    }



    void Renderer2D::draw_line(Vector2<float> p1, Vector2<float>p2, SDL_Color color, bool screen_space)
    {
        #if !IS_SERVER
            if(!screen_space)
            {
                p1.x *= world_unit_to_pixels*zoom_amount;
                p1.y *= world_unit_to_pixels*zoom_amount;
                p2.x *= world_unit_to_pixels*zoom_amount;
                p2.y *= world_unit_to_pixels*zoom_amount;
            }
        #endif
        //debug_shapes_line.emplace(color, p1,p2 );

    }

    SDL_Rect Renderer2D::get_world_to_screen_rect(SDL_FRect rect)
    {
        SDL_Rect srect;
        rect.x -= view_position.x;
        rect.y -= view_position.y;

        srect.x = (rect.x * world_unit_to_pixels * zoom_amount) + window_width/2;
        srect.y = (-(rect.y * world_unit_to_pixels * zoom_amount)) + window_height/2;
        srect.h = rect.h * world_unit_to_pixels * zoom_amount;
        srect.w = rect.w * world_unit_to_pixels * zoom_amount;

        return srect;
    }

    // private    
    void Renderer2D::draw()
    {
        #if !IS_SERVER
            //insertionSort(render_list, render_array_head);
            for(int _i=0; _i<=render_array_head; _i++)
            {
                Graphics* _graphics = render_list[_i];
                if(_graphics != nullptr)
                {
                    _graphics->draw(this, true);
                }
            }
        #endif
    }

    ///////////////////////////////TEXT RENDERING//////////////////////////////////////////
    TTF_Font* Renderer2D::open_font(const std::string font_file_name, int font_size)
    {
        #if !IS_SERVER
            TTF_Font *font = TTF_OpenFont(font_file_name.c_str(), font_size);
            if (font == nullptr)
            {
                LOG( LOGTYPE_ERROR, "Error openting font ",font_file_name, SDL_GetError());
                return nullptr;
            }
            else
            {
                LOG( LOGTYPE_GENERAL, "loaded font :", font_file_name.c_str());
                return font;
            }
        #else
            return nullptr;
        #endif
    }

    void Renderer2D::close_font(TTF_Font* font)
    {
        #if !IS_SERVER
            if(font != nullptr){
                TTF_CloseFont(font);
            }
        #endif
    }

    SDL_Texture* Renderer2D::get_text_texture(const std::string& text, TTF_Font* font, SDL_Color& color)
    {
        #if !IS_SERVER
            SDL_Surface *surf = TTF_RenderText_Blended(font, text.c_str(), color);
            if (surf == nullptr)
            {
                TTF_CloseFont(font);
                LOG( LOGTYPE_ERROR,"rendering font to surface failed: ", SDL_GetError());
                LOG( LOGTYPE_ERROR,"drawing ttf font failed");
                return nullptr;
            }
            SDL_Texture *texture = SDL_CreateTextureFromSurface(sdl_renderer, surf);
            SDL_FreeSurface(surf);

            if (texture == nullptr){
                LOG( LOGTYPE_ERROR,"creating texture from text surface failed: ", SDL_GetError());
                return nullptr;
            }

            return texture;
        #else
            return nullptr;
        #endif

    }

    void Renderer2D::draw_text(SDL_Texture* texture, SDL_Rect* dst_region, TextJustification text_justification, int height)
    {
        #if !IS_SERVER
            SDL_Rect tex_dim;
            SDL_QueryTexture(texture, NULL, NULL, &tex_dim.w, &tex_dim.h);
            int c = dst_region->h/2 + dst_region->y;
            SDL_Rect source_dim{0,0,dst_region->w, dst_region->h};
            float factor = tex_dim.h/(float)height;

            source_dim.x = tex_dim.w-(source_dim.w*factor) ;
            if(source_dim.x < 0)
            {
                source_dim.x = 0;
                source_dim.w = tex_dim.w;
                dst_region->w = tex_dim.w/factor;
            }
            else
            {
                source_dim.w *= factor;
            }
            
            source_dim.h = tex_dim.h;
            dst_region->h = tex_dim.h/factor;
            dst_region->y = c;
            justify_text(text_justification, *dst_region);
            
            SDL_RenderCopy(sdl_renderer, texture, &source_dim, dst_region);
            tex_dim.x = dst_region->x;
            tex_dim.y = dst_region->y;
            //set_draw_color(SDL_Color{166,155,155,255});
            //SDL_RenderDrawRect(sdl_renderer, &tex_dim);
            //set_draw_color(SDL_Color{66,55,55,255});
            //SDL_RenderDrawRect(sdl_renderer, dst_region);
        #endif
    }

    void Renderer2D::draw_text(const std::string& text, TTF_Font* font, SDL_Color& color,Vector2<int> screen_position, TextJustification text_justification)
    {
        #if !IS_SERVER
            SDL_Texture *texture =  get_text_texture(text,font,color);
            if(texture != nullptr)
            {
                draw_text(texture, screen_position, text_justification);
                SDL_DestroyTexture(texture);
            }
        #endif
    }

    void Renderer2D::draw_text(SDL_Texture* texture, Vector2<int> screen_position, TextJustification text_justification, int height)
    {
        #if !IS_SERVER
            SDL_Rect text_rect;
            text_rect.x = screen_position.x;
            text_rect.y = screen_position.y;
            SDL_QueryTexture(texture, NULL, NULL, &text_rect.w, &text_rect.h);

            if(height != -1)
            {
                text_rect.w = ((height/(float)text_rect.h)*text_rect.w);
                text_rect.h = height;
            }

            justify_text(text_justification, text_rect);
            SDL_RenderCopy(sdl_renderer, texture, nullptr, &text_rect);
        #endif
    }




    SDL_Texture* Renderer2D::draw_text_debug(const std::string& text, TTF_Font* font, SDL_Color& color, Vector2<int> screen_position, TextJustification text_justification)
    {
        SDL_Texture *texture =  get_text_texture(text,font,color);

        SDL_Rect text_rect;
        text_rect.x = screen_position.x;
        text_rect.y = screen_position.y;
        SDL_QueryTexture(texture, NULL, NULL, &text_rect.w, &text_rect.h);
        justify_text(text_justification, text_rect);

        ttf_textures.push( std::pair<SDL_Texture*, SDL_Rect>{texture, text_rect});
        return texture;
    }

    void Renderer2D::justify_text(TextJustification tj, SDL_Rect& text_rect)
    {
        #if !IS_SERVER
            // based on justification, define text texture rect size and position
            switch (tj)
            {
            case TOP_LEFT:
                /* code */
                break;
            case TOP_CENTER:
                text_rect.x = (text_rect.x - text_rect.w/2);
                //text_rect.y = (screen_position.y - _h/2);
                break;
            case TOP_RIGHT:
                text_rect.x = (text_rect.x - text_rect.w);            
                /* code */
                break;
            case MIDDLE_LEFT:
                text_rect.y = (text_rect.y - text_rect.h/2);
                /* code */
                break;
            case MIDDLE_CENTER:
                text_rect.x = (text_rect.x - text_rect.w/2);
                text_rect.y = (text_rect.y - text_rect.h/2);
                /* code */
                break;
            case MIDDLE_RIGHT:
                text_rect.x = (text_rect.x - text_rect.w);            
                text_rect.y = (text_rect.y - text_rect.h/2);

                /* code */
                break;
            case BOTTOM_LEFT:
                text_rect.y = (text_rect.y - text_rect.h);
                /* code */
                break;
            case BOTTOM_CENTER:
                text_rect.x = (text_rect.x - text_rect.w/2);
                text_rect.y = (text_rect.y - text_rect.h);

                /* code */
                break;
            case BOTTOM_RIGHT:
                text_rect.x = (text_rect.x - text_rect.w);            
                text_rect.y = (text_rect.y - text_rect.h);

                /* code */
                break;
            default:
                break;
            }
        #endif
    }


    void Renderer2D::draw_debug()
    {
        #if !IS_SERVER
            while(!debug_shapes_rect.empty())
            {
                SDL_Color &c = debug_shapes_rect.front().first;
                SDL_Rect &rect = debug_shapes_rect.front().second; 
                set_draw_color(c);
                SDL_RenderDrawRect( sdl_renderer , &rect);
                debug_shapes_rect.pop();
            }

            while(!debug_shapes_line.empty())
            {
                SDL_Color &c = debug_shapes_line.front().first;
                std::pair<Vector2<float>, Vector2<float>> &line = debug_shapes_line.front().second; 
                set_draw_color(c);
                #ifndef EMSCRIPTEN
                SDL_RenderDrawLineF(sdl_renderer, line.first.x, line.first.y, line.second.x, line.second.y);
                #else
                SDL_RenderDrawLine(sdl_renderer, (int)line.first.x, (int)line.first.y, (int)line.second.x, (int)line.second.y);
                #endif
                debug_shapes_line.pop();
            }
        #endif
    }

    void Renderer2D::draw_ttf_textures()
    {
        #if !IS_SERVER
            while(!ttf_textures.empty())
            {
                SDL_RenderCopy(sdl_renderer, ttf_textures.front().first, NULL, &ttf_textures.front().second);
                SDL_DestroyTexture(ttf_textures.front().first);
                ttf_textures.pop();
            }
        #endif
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////GRAPHICS CLASS DEFINTION///////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    Graphics::Graphics():Component(SPRITE)
    {
        graphics_frames = nullptr;
        this->current_frame_index = -1;
        this->number_of_frames = 0;
        this->angle = 0;
    }

    Graphics::~Graphics()
    {
        if(graphics_frames != nullptr)
        {
            /*for(int _i=0; _i < number_of_frames; _i++)
            {
                if(graphics_frames[_i].texture != nullptr)
                    SDL_DestroyTexture(graphics_frames[_i].texture);
            }*/
            //LOG(LOGTYPE_GENERAL,"Deleted: ");
            delete[] graphics_frames;
        }
    }
    
    bool Graphics::load_graphics(std::string graphics_file, const TextureSlicingParameters* slicing_params, const Renderer2D& renderer2d, int num_of_frames, bool is_subregion)
    {
        #if !IS_SERVER
            // load all animation frames
            if(slicing_params == nullptr)
            {   
                LOG(LOGTYPE_ERROR, "Slicing parameter is null, can't load graphics");
                return false;
            }

            this->number_of_frames = num_of_frames;
            graphics_frames = new GraphicsFrame[num_of_frames];
            SDL_Texture* texture = PrEngine::load_texture(graphics_file, renderer2d.sdl_renderer);
            
            if(texture != nullptr)
            {
                int w = 0;
                int h = 0;
                SDL_QueryTexture(texture, NULL, NULL, &w, &h);
                //LOG(LOGTYPE_GENERAL, std::to_string(w).append("  ").append(std::to_string(h)));

                int columns = w/(slicing_params->w+slicing_params->x_pad);
                int rows = h/(slicing_params->h+slicing_params->y_pad);
                int frame_no = 0;
                int _i= slicing_params->x + slicing_params->x_pad;

                for(int _j = slicing_params->y + slicing_params->y_pad; _j<h; _j += (slicing_params->h+slicing_params->y_pad))
                {
                    for(; _i<w; _i += (slicing_params->w+slicing_params->x_pad))
                    {
                        if(frame_no<this->number_of_frames)
                        {
                            GraphicsFrame* frame = &graphics_frames[frame_no];
                            frame->texture = texture;
                            frame->region.x = _i;
                            frame->region.y = _j;
                            
                            if(_i+slicing_params->w > w)
                                frame->region.w = slicing_params->w - (_i+slicing_params->w-w);
                            else
                                frame->region.w = slicing_params->w;

                            if(_j+slicing_params->h > h)
                                frame->region.h = slicing_params->h - (_j+slicing_params->h-h);
                            else
                                frame->region.h = slicing_params->h;
                            
                            frame_no++;
                        }
                    }
                    if(is_subregion)
                        _i= slicing_params->x + slicing_params->x_pad;
                    else
                        _i = slicing_params->x_pad;
                }

                if(frame_no < number_of_frames)
                {    
                    LOG(LOGTYPE_ERROR, "Not enough frames in source texture, skipped ", std::to_string(number_of_frames-frame_no)," frames");
                    number_of_frames = frame_no;
                }
                current_frame_index = 0;
                return true;
            }
            LOG(LOGTYPE_ERROR, "Texture is null, couldn't load graphics");
            return false;
        #endif
    }

    bool Graphics::load_graphics(SDL_Texture* texture, const TextureSlicingParameters* slicing_params, const Renderer2D& renderer2d, int num_of_frames, bool is_subregion)
    {
        #if !IS_SERVER
            // load all animation frames
            if(slicing_params == nullptr)
            {   
                LOG(LOGTYPE_ERROR, "Slicing parameter is null, can't load graphics");
                return false;
            }

            this->number_of_frames = num_of_frames;
            graphics_frames = new GraphicsFrame[num_of_frames];
            if(texture != nullptr)
            {
                int w = 0;
                int h = 0;
                SDL_QueryTexture(texture, NULL, NULL, &w, &h);
                //LOG(LOGTYPE_GENERAL, std::to_string(w).append("  ").append(std::to_string(h)));

                int frame_no = 0;

                int _i= slicing_params->x + slicing_params->x_pad;
                for(int _j = slicing_params->y + slicing_params->y_pad; _j<h; _j += (slicing_params->h+slicing_params->y_pad))
                {
                    for(; _i<w; _i += (slicing_params->w+slicing_params->x_pad))
                    {
                        if(frame_no<this->number_of_frames)
                        {
                            GraphicsFrame* frame = &graphics_frames[frame_no];
                            frame->texture = texture;
                            frame->region.x = _i;
                            frame->region.y = _j;
                            
                            if(_i+slicing_params->w > w)
                                frame->region.w = slicing_params->w - (_i+slicing_params->w-w);
                            else
                                frame->region.w = slicing_params->w;

                            if(_j+slicing_params->h > h)
                                frame->region.h = slicing_params->h - (_j+slicing_params->h-h);
                            else
                                frame->region.h = slicing_params->h;

                            frame_no++;
                        }
                    }
                    if(is_subregion)
                        _i= slicing_params->x + slicing_params->x_pad;
                    else
                        _i = slicing_params->x_pad;
                }

                if(frame_no < number_of_frames)
                {    
                    LOG(LOGTYPE_ERROR, "Not enough frames in source texture, skipped ", std::to_string(number_of_frames-frame_no)," frames");
                    number_of_frames = frame_no;
                }

                current_frame_index = 0;
                return true;
            }
            LOG(LOGTYPE_ERROR, "Texture is null, couldn't load graphics");
            return false;
        #endif
    }

    void Graphics::add_frame_to_graphics(const GraphicsFrame& frame)
    {
        this->number_of_frames++;
        GraphicsFrame* _temp = new GraphicsFrame[this->number_of_frames];
        for(int i=0; i<number_of_frames-1; i++)
        {
            _temp[i].texture = graphics_frames[i].texture;
            _temp[i].region = graphics_frames[i].region;
        }
        _temp[number_of_frames-1].region = frame.region;
        _temp[number_of_frames-1].texture = frame.texture;
        
        delete[] graphics_frames;
        graphics_frames = _temp;
    }



    GraphicsFrame* Graphics::get_current_frame()
    {
        if(graphics_frames != nullptr)
            return &this->graphics_frames[this->current_frame_index];
        else
            return nullptr;
    }

    GraphicsFrame* Graphics::get_frame_at(int index)
    {
        if(index>=number_of_frames)
            LOG(LOGTYPE_ERROR, "index is greater than frame count");
        if(graphics_frames != nullptr)  
            return &this->graphics_frames[index];
        return nullptr;
    }  

    void Graphics::draw(Renderer2D* renderer, bool centered)
    {
            // dst_dimension has world unit values, convert to screen space
            // also make drawing relative to camera position
            // also make center of screen 0,0
            //dst_dimension.w *= scale;
            //dst_dimension.h *= scale;
            
            dst_dimension.x = dst_dimension.x - (centered * dst_dimension.w/2);// +  world_space*renderer->window_width/2);
            dst_dimension.y = dst_dimension.y - (centered * dst_dimension.h/2);//+ world_space*renderer->window_height/2);
            
            SDL_RenderCopyEx(renderer->sdl_renderer, get_current_frame()->texture , &(get_current_frame()->region), 
                                    &dst_dimension, angle, NULL, SDL_FLIP_NONE) ;
    }
}