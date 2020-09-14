#include "GUI.hpp"


namespace PrEngine
{
    Mouse* mouse = nullptr;
    Renderer2D* renderer = nullptr;
    Graphics gui_graphics[256];
    UI hot{0};
    UI active{0};
    int canvas_width = 1280;
    int canvas_height = 720;
    std::map<uintptr_t,SDL_Texture*> rendered_texts;
    TTF_Font* fonts[FONT_ID_MAX];
    SDL_Event event;
    int text_box_width_in_chars = 0;

	
    void show_file_structure(std::string root, std::string prefix)
    {
        
        #ifdef _WIN64
            const std::string PATH_SEP = "\\";
        #else
            const std::string PATH_SEP = "/";
        #endif

        DIR *dir;
        struct dirent *ent;
        if ((dir = opendir (root.c_str())) != NULL) 
        {
            /* print all the files and directories within directory */
            while ((ent = readdir (dir)) != NULL) 
            {
                std::cout<<prefix<<"|"<<ent->d_name<<std::endl;
                //if(ent->d_type == DT_DIR && ent->d_name[0] != '.' )
                //    show_file_structure( root.append(ent->d_name).append(PATH_SEP), prefix.append(" "));
            }
            closedir (dir);
        } 
        else
        {
            /* could not open directory */
            perror ("");
            return;
        }
    }
	

    bool initialize_gui(Mouse* _mouse, Renderer2D* _renderer)
    {
        // load all the fonts that we'll need   
        for(int i=0;i <FONT_ID_MAX; i++)
            fonts[i] = nullptr;

        fonts[FONT_SAMPLE_TTF_16] = renderer->open_font(get_resource_path("sample.ttf"), 16);
        fonts[FONT_SAMPLE_TTF_24] = renderer->open_font(get_resource_path("sample.ttf"), 24);
        fonts[FONT_SAMPLE_TTF_32] = renderer->open_font(get_resource_path("sample.ttf"), 32);
        fonts[FONT_SAMPLE_TTF_48] = renderer->open_font(get_resource_path("sample.ttf"), 48);
        
        ///////////////////////////////////////
        PrEngine::mouse = _mouse;
        PrEngine::renderer = _renderer;
        int error_flag = 0;

        // load all the texture we'll need
        SDL_Texture* _texture = load_texture( get_resource_path("UIpack/PNG/blue_panel.png"), renderer->sdl_renderer);
        TextureSlicingParameters slicing_params(5,10,20,20,0,0);
        
        if(!gui_graphics[GUI_ID_PANEL].load_graphics(_texture, &slicing_params, *(renderer), 1))
        {
            error_flag = 1;
            LOG(LOGTYPE_GENERAL, "Failed to load panel graphics. Texture: ","blue_panel.png");
        }

        _texture = load_texture( get_resource_path("UIpack/PNG/green_panel.png"), renderer->sdl_renderer);
        if(!gui_graphics[GUI_ID_PANEL_2].load_graphics(_texture, &slicing_params, *(renderer), 1))
        {
            error_flag = 1;
            LOG(LOGTYPE_GENERAL, "Failed to load panel graphics. Texture: ","green_panel.png");
        }


        _texture = load_texture( get_resource_path("UI_elements.png"), renderer->sdl_renderer);
        slicing_params = TextureSlicingParameters(96,0,96,32,0,0);
        if(!gui_graphics[GUI_ID_BUTTON].load_graphics(_texture, &slicing_params, *(renderer), 3, true))
        {
            error_flag = 1;
            LOG(LOGTYPE_GENERAL, "Failed to load button graphics. Texture: ","UI_elements.png");
        }

        _texture = load_texture( get_resource_path("UIpack/PNG/grey_sliderHorizontal.png"), renderer->sdl_renderer);
        slicing_params = TextureSlicingParameters(0,0,190,4,0,0);
        if(!gui_graphics[GUI_ID_SLIDER_BASE].load_graphics(_texture, &slicing_params, *(renderer),1))
        {
            error_flag = 1;
            LOG(LOGTYPE_GENERAL, "Failed to load slider background graphics. Texture: ","UI_elements.png");
        }

        _texture = load_texture( get_resource_path("UIpack/PNG/grey_sliderRight.png"), renderer->sdl_renderer);
        slicing_params = TextureSlicingParameters(0,0,39,31,0,0);
        if(!gui_graphics[GUI_ID_SLIDER_KNOB].load_graphics(_texture, &slicing_params, *(renderer),1))
        {
            error_flag = 1;
            LOG(LOGTYPE_GENERAL, "Failed to load slider_knob graphics. Texture: ","UI_elements.png");
        }


        _texture = load_texture( get_resource_path("UIpack/PNG/blue_button03.png"), renderer->sdl_renderer);
        slicing_params = TextureSlicingParameters(0,0,190,45,0,0);
        if(!gui_graphics[GUI_ID_TEXTBOX].load_graphics(_texture, &slicing_params, *(renderer),1))
        {
            error_flag = 1;
            LOG(LOGTYPE_GENERAL, "Failed to load text box graphics. Texture: ","UI_elements.png");
        }
        _texture = load_texture( get_resource_path("UIpack/PNG/green_button03.png"), renderer->sdl_renderer);

        GraphicsFrame gf;
        gf.region = SDL_Rect{0,0,190,45};
        gf.texture =_texture;
        gui_graphics[GUI_ID_TEXTBOX].add_frame_to_graphics(gf);

        _texture = load_texture( get_resource_path("UIpack/PNG/grey_box.png"), renderer->sdl_renderer);
        slicing_params = TextureSlicingParameters(0,0,38,36,0,0);
        if(!gui_graphics[GUI_ID_TOGGLE].load_graphics(_texture, &slicing_params, *(renderer),1))
        {
            error_flag = 1;
            LOG(LOGTYPE_GENERAL, "Failed to load toggle graphics. Texture: ","grey_box.png");
        }

        _texture = load_texture( get_resource_path("UIpack/PNG/green_boxCheckmark.png"), renderer->sdl_renderer);
        if(_texture == nullptr)
        {
            error_flag = 1;
            LOG(LOGTYPE_GENERAL, "Failed to load toggle graphics. Texture: ","green_boxCheckmark.png");
        }
        GraphicsFrame gf2;
        gf2.region = SDL_Rect{0,0,38,36};
        gf2.texture =_texture;
        gui_graphics[GUI_ID_TOGGLE].add_frame_to_graphics(gf2);


        //LOG(LOGTYPE_WARNING, "New len of graphics frames ", std::to_string(gui_graphics[GUI_ID_TEXTBOX].number_of_frames));
        return error_flag;
    }

    void draw_panel(uintptr_t id, SDL_Rect& draw_region, SDL_FRect& anchor,GUI_ID gui_id)
    {
        //render
        process_rect_with_anchor(draw_region, anchor, renderer->window_width, renderer->window_height);
        gui_graphics[gui_id].dst_dimension = draw_region;
        gui_graphics[gui_id].draw(renderer,false);
    }

    bool do_button(uintptr_t id, SDL_Rect& draw_region, SDL_FRect& anchor, const std::string& text, const TextJustification& justification)
    {
        process_rect_with_anchor(draw_region, anchor, renderer->window_width, renderer->window_height);
        gui_graphics[GUI_ID_BUTTON].dst_dimension = draw_region;
        //renderer->draw_rectangle(graphics->dst_dimension, SDL_Color{255,0,0,255},true, true);
        //LOG(LOGTYPE_GENERAL, std::to_string(graphics->dst_dimension.x),", ", std::to_string(graphics->dst_dimension.y),", ", std::to_string(graphics->dst_dimension.w),", ", std::to_string(graphics->dst_dimension.h));
        gui_graphics[GUI_ID_BUTTON].current_frame_index = 0;
        if(hot.id == id) gui_graphics[GUI_ID_BUTTON].current_frame_index = 1;
        if(active.id == id) gui_graphics[GUI_ID_BUTTON].current_frame_index = 2;
        gui_graphics[GUI_ID_BUTTON].draw(renderer, false);

        if(active.id == id) 
        {
            if(hot.id == id)
            {
                if(mouse->get_mouse_button_up(1))
                {
                    active.id = 0;
                    return true;
                }
            }
        }
        else
        {
            if(hot.id == id)
            {
                if(mouse->get_mouse_button_down(1))
                {
                    active.id = id;
                }
            }
        }

        //LOG(LOGTYPE_GENERAL, std::to_string(mouse->position.x),std::to_string(mouse->position.y));
        if(inside(mouse->position, gui_graphics[GUI_ID_BUTTON].dst_dimension))
        {
            hot.id = id;
        }
        else if(hot.id == id)
        {
            hot.id = 0;
            active.id = 0;
        }
        return false;
    }

    bool do_toggle(uintptr_t id,bool val, SDL_Rect& draw_region, SDL_FRect& anchor)
    {
        //LOG(LOGTYPE_GENERAL, std::to_string(val));
        process_rect_with_anchor(draw_region, anchor, renderer->window_width, renderer->window_height);

        gui_graphics[GUI_ID_TOGGLE].dst_dimension = draw_region;
        gui_graphics[GUI_ID_TOGGLE].current_frame_index = 0;
        gui_graphics[GUI_ID_TOGGLE].draw(renderer, false);

        if(val) 
        {
            gui_graphics[GUI_ID_TOGGLE].current_frame_index = 1;
            gui_graphics[GUI_ID_TOGGLE].draw(renderer,false);
        }

        if(inside(mouse->position, gui_graphics[GUI_ID_TOGGLE].dst_dimension))
        {
            if(mouse->get_mouse_button_down(1))
            {
                val = !val;
            }
        }
        return val;
    }


    float get_slider(uintptr_t id, float value, SDL_Rect& slider_base_region, SDL_Rect& slider_knob_region, SDL_FRect& anchor, float min, float max, bool horizontal)
    {
        float _h = slider_base_region.h;
        process_rect_with_anchor(slider_base_region, anchor, renderer->window_width, renderer->window_height);
        _h = _h/(float)slider_base_region.h;
        gui_graphics[GUI_ID_SLIDER_BASE].dst_dimension = slider_base_region;
        gui_graphics[GUI_ID_SLIDER_BASE].draw(renderer, false);
        if(active.id == id) 
        {
            if(mouse->get_mouse_button(1))
            {
                //active.id = 0;
                value = (mouse->position.x - gui_graphics[GUI_ID_SLIDER_BASE].dst_dimension.x)/(float)gui_graphics[GUI_ID_SLIDER_BASE].dst_dimension.w;
                value = clamp(value,min,max);
            }
            if(mouse->get_mouse_button_up(1))
            {
                active.id = 0;
            }
        }
        else
        {
            if(hot.id == id)
            {
                if(mouse->get_mouse_button_down(1))
                {
                    active.id = id;
                }
            }
        }


        if(inside(mouse->position, gui_graphics[GUI_ID_SLIDER_BASE].dst_dimension))
        {
            hot.id = id;
        }
        else if(hot.id == id)
        {
            hot.id = 0;
        }

        slider_knob_region.x =  slider_base_region.x + slider_base_region.w*value;
        float _s = (gui_graphics[GUI_ID_SLIDER_BASE].dst_dimension.h * 3)/slider_knob_region.h;
        slider_knob_region.h = _s * slider_knob_region.h;
        slider_knob_region.w = _s * slider_knob_region.w;
        slider_knob_region.y = slider_base_region.y + slider_base_region.h/2.f - slider_knob_region.h/2.f;
        gui_graphics[GUI_ID_SLIDER_KNOB].dst_dimension = slider_knob_region;
        gui_graphics[GUI_ID_SLIDER_KNOB].draw(renderer, false);
        return value;
    }

    
    void show_text(uintptr_t id, std::string& text, SDL_Rect& draw_region, SDL_FRect& anchor,Font_ID font_id, SDL_Color& c, float scale)
    {
        if(text.length() >0)
        {
            if( fonts[font_id] != nullptr)
            {
                SDL_Texture* texTure = renderer->get_text_texture(text, fonts[font_id], c);
                process_rect_with_anchor(draw_region, anchor, renderer->window_width, renderer->window_height);
                renderer->draw_text(texTure, Vector2<int>(draw_region.x+draw_region.w/2,draw_region.y+draw_region.h/2), MIDDLE_CENTER, draw_region.h*scale);
                SDL_DestroyTexture(texTure);
            }   
            else
            {
                LOG(LOGTYPE_ERROR, "Font hasn't been loaded; please load in load_fonts() ");
            }
        }
    }

    void get_text_input(uintptr_t id, std::string& text, SDL_Rect& text_box_region, SDL_FRect& anchor)
    {
        process_rect_with_anchor(text_box_region, anchor, renderer->window_width, renderer->window_height);
        gui_graphics[GUI_ID_TEXTBOX].dst_dimension = text_box_region;
        gui_graphics[GUI_ID_TEXTBOX].current_frame_index = 0;
        if(active.id == id) gui_graphics[GUI_ID_TEXTBOX].current_frame_index = 1;
        gui_graphics[GUI_ID_TEXTBOX].draw(renderer,false);

        SDL_Color c{0,0,0,255};
        //SDL_FRect tb_a = {(text_box_region.x+text_box_region.w/2.f)/(float)canvas_width, anchor.y,
        //                    0.f, anchor.h};
        //show_text(id, text, text_box_region, tb_a, FONT_SAMPLE_TTF_32,c, 0.6f);
        
        //LOG(LOGTYPE_GENERAL, std::to_string(text_box_width_in_chars));
        //std::string sub_text = text;

        if(text.length() > 0)
        {
            //if(text_box_width_in_chars < sub_text.length() && text_box_width_in_chars>0)
            //    sub_text = text.substr(text.length()-text_box_width_in_chars, text_box_width_in_chars);

            SDL_Texture* texTure = renderer->get_text_texture(text, fonts[FONT_SAMPLE_TTF_48], c);

            renderer->draw_text(texTure,&text_box_region, MIDDLE_LEFT, text_box_region.h*0.6f);
            //if(text_box_width_in_chars==0) text_box_width_in_chars = (text_box_region.w/width_of_chars);
            SDL_DestroyTexture(texTure);
            
        }


        if(active.id == id) 
        {
            //if(textinput_modified)
            text = textinput;
            if(mouse->get_mouse_button_down(1))
                if(hot.id != id)
                    active.id = 0;
        }
        else
        {
            if(hot.id == id)
            {
                if(mouse->get_mouse_button_down(1))
                {
                    textinput = text;
                    active.id = id;
                }
            }
        }

        if(inside(mouse->position, gui_graphics[GUI_ID_TEXTBOX].dst_dimension))
        {
            hot.id = id;
        }
        else if(hot.id == id)
        {
            hot.id = 0;
        }

    }

    void process_rect_with_anchor(SDL_Rect& rect, SDL_FRect& anchor, int width, int height)
    {
        // anchor to pixel position in original canvas 
        Vector2<int> p1_c((int)(anchor.x*canvas_width)  , (int)(anchor.y*canvas_height));
        Vector2<int> p2_c((int)((anchor.x+anchor.w)*canvas_width), (int)((anchor.y+anchor.h)*canvas_height));

        Vector2<int> p1_s((int)(anchor.x*width)  , (int)(anchor.y*height));
        Vector2<int> p2_s((int)((anchor.x+anchor.w)*width), (int)((anchor.y+anchor.h)*height));

        Vector2<int>r1(rect.x, rect.y);
        Vector2<int>r2(rect.x+rect.w, rect.y+rect.h);
        //LOG(LOGTYPE_GENERAL, std::to_string(p1_c.x),",",std::to_string(p1_s.x));
        //LOG(LOGTYPE_GENERAL, std::to_string(p2_c.x),",",std::to_string(p2_s.x));
        //       p1.......p2.x,p1.y
        //       |        |
        // p1.x,p2.y......p2


//          (x,y)........(x+w, y)
//              |        |
//        (x,y+h)........(x+w, y+h)

        Vector2<int>f1( (r1.x - p1_c.x) + p1_s.x,  (r1.y - p1_c.y) + p1_s.y )  ;
        Vector2<int>f2( (r2.x - p2_c.x) + p2_s.x,  (r2.y - p2_c.y) + p2_s.y )  ;

        rect.x = f1.x;
        rect.y = f1.y;
        rect.w = f2.x - f1.x;
        rect.h = f2.y - f1.y;
    }

    void get_anchor_full(SDL_Rect& region, SDL_FRect& anchor)
    {
        anchor.x = region.x/(float)canvas_width;
        anchor.y = region.y/(float)canvas_height;
        anchor.w = region.w/(float)canvas_width;
        anchor.h = region.h/(float)canvas_height;
    }

    void get_anchor_const_width(SDL_Rect& region, SDL_FRect& anchor)
    {
        anchor.x = (region.x+region.w/2.f)/(float)canvas_width;
        anchor.y = region.y/(float)(float)canvas_height;
        anchor.w = 0.f;
        anchor.h = region.h/(float)(float)canvas_height;
    }

    void get_anchor_const_height(SDL_Rect& region, SDL_FRect& anchor)
    {
        anchor.x = region.x/(float)canvas_width;
        anchor.y = (region.y+region.h/2.f)/(float)canvas_height;
        anchor.w = region.w/(float)canvas_width;
        anchor.h = 0.f;
    }
    
}
