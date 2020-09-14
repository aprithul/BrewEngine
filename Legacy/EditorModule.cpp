#include "EditorModule.hpp"
namespace PrEngine
{
            
    SDL_Rect right_panel_outer_region;
    SDL_Rect right_panel_inner_region;
    SDL_FRect right_panel_outer_anchor;
    SDL_FRect right_panel_inner_anchor;

    SDL_Rect left_panel_outer_region;
    SDL_Rect left_panel_inner_region;
    SDL_FRect left_panel_outer_anchor;
    SDL_FRect left_panel_inner_anchor;

    SDL_Rect left_panel_text_0_region;
    SDL_Rect left_panel_text_1_region;
    SDL_Rect left_panel_text_2_region;

    SDL_FRect left_panel_text_0_anchor;
    SDL_FRect left_panel_text_1_anchor;
    SDL_FRect left_panel_text_2_anchor;

    uintptr_t editor_panel_id_0 = 0;
    uintptr_t editor_panel_id_1 = 1;
    uintptr_t editor_panel_id_2 = 2;
    uintptr_t editor_panel_id_3 = 3;
    uintptr_t editor_text_id_4 = 4;
    uintptr_t editor_text_id_5 = 5;
    uintptr_t editor_text_id_6 = 6;
    


    Editor::Editor(std::string name, int priority, Renderer2D* renderer):Module(name,priority)
    {
        this->renderer = renderer;

        right_panel_outer_region = {canvas_width - (canvas_width/8), 0, (canvas_width/8),canvas_height};
        get_anchor_full(right_panel_outer_region, right_panel_outer_anchor);
        right_panel_inner_region = {canvas_width - (canvas_width/8)+5, 5, (canvas_width/8)-10,canvas_height-10};
        get_anchor_full(right_panel_outer_region, right_panel_inner_anchor);

        left_panel_outer_region = {0, 0, (canvas_width/8),canvas_height};
        get_anchor_full(left_panel_outer_region, left_panel_outer_anchor);
        left_panel_inner_region = {5, 5, (canvas_width/8)-10,canvas_height-10};
        get_anchor_full(left_panel_outer_region, left_panel_inner_anchor);

    }

    Editor::~Editor()
    {

    }

    Sprite* sprite_cube1;
    Sprite* sprite_cube2;
    Sprite* sprite_cube3;

    Camera* camera;
    long selected_entity_id = -1;
    Vector2<float> selection_offset;

    void Editor::start()
    {
        PrEngine::TextureSlicingParameters slicing_param_2(0,0,1024,1024,0,0);
        sprite_cube1 = new Sprite(PrEngine::get_resource_path("cube.png"), &slicing_param_2, *renderer, 1, 100, 9);
        sprite_cube1->transform.position = PrEngine::Vector2<float>(4,2);
        sprite_cube1->transform.scale = PrEngine::Vector2<float>(0.5f,0.5f);
        sprite_cube1->set_layer(15);
        LOG(LOGTYPE_GENERAL, "Editor started");
        PrEngine::entity_management_system->assign_id_and_store_entity(*sprite_cube1);

       /*         sprite_cube2 = new Sprite(PrEngine::get_resource_path("cube.png"), &slicing_param_2, *renderer, 1, 100, 12);
        sprite_cube2->transform.position = PrEngine::Vector2<float>(0,0);
        sprite_cube2->transform.scale = PrEngine::Vector2<float>(0.5f,0.5f);
        LOG(LOGTYPE_GENERAL, "Editor started");
       // sprite_cube1->set_layer(0);

        PrEngine::entity_management_system->assign_id_and_store_entity(*sprite_cube2);


                sprite_cube2 = new Sprite(PrEngine::get_resource_path("cube.png"), &slicing_param_2, *renderer, 1, 100, 11);
        sprite_cube2->transform.position = PrEngine::Vector2<float>(-6.2f,-2.8f);
        sprite_cube2->transform.scale = PrEngine::Vector2<float>(0.5f,0.5f);
        LOG(LOGTYPE_GENERAL, "Editor started");
        //sprite_cube1->set_layer(2);
        
        PrEngine::entity_management_system->assign_id_and_store_entity(*sprite_cube2);*/
        

        camera = (Camera*)entity_management_system->get_entity(ENTITY_TYPE_CAMERA);
    }

    void Editor::update()
    {
        //// EDITOR GUI DRAWING ///////////
        // left panel
        left_panel_outer_region = {0, 0, (canvas_width/8),canvas_height};
        draw_panel(editor_panel_id_2, left_panel_outer_region, left_panel_outer_anchor, GUI_ID_PANEL_2);
        left_panel_inner_region = {5, 5, (canvas_width/8)-10,canvas_height-10};
        draw_panel(editor_panel_id_3, left_panel_inner_region, left_panel_inner_anchor);

        // right panel
        right_panel_outer_region = {canvas_width - (canvas_width/8), 0, (canvas_width/8),canvas_height};
        draw_panel(editor_panel_id_0, right_panel_outer_region, right_panel_outer_anchor, GUI_ID_PANEL_2);
        right_panel_inner_region = {canvas_width - (canvas_width/8)+5, 5, (canvas_width/8)-10,canvas_height-10};
        draw_panel(editor_panel_id_1, right_panel_inner_region, right_panel_inner_anchor);

        ////////////////////////////////////
        Vector2<float> wsp = camera->get_screen_to_world_position(input_manager->mouse.position);
        if(input_manager->mouse.get_mouse_button_down(1))
        {
            for(int i=0;i <= entity_management_system->current_max_entity_pos;i++)
            {
                Entity* et = entity_management_system->entities[i];
                if(et)
                {
                    if(et->type == ENTITY_TYPE_SPRITE)
                    {
                        if(inside( wsp, ((Sprite*)et)->world_space_bounds,true))
                        {
                            selected_entity_id = et->id;
                            selection_offset = wsp - Vector2<float>( ((Sprite*)et)->world_space_bounds.x,((Sprite*)et)->world_space_bounds.y);
                        }
                    }
                }
            }
        }
        else if(input_manager->mouse.get_mouse_button_up(1))
        {
            selected_entity_id = -1;
        }
        else
        {
            if(selected_entity_id!=-1)
            {
                Sprite* sp = (Sprite*)entity_management_system->get_entity(selected_entity_id);
                if(sp)
                    sp->transform.position = wsp - selection_offset;

            }
        }

    }

    void Editor::end()
    {
        
    }

}
