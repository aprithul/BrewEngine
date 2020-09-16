//  main.cpp
//  Game Engine
//
//  Created by Aniruddha Prithul on 7/22/17.
//  Copyright © 2017 Aniruddha Prithul. All rights reserved.


#include <stdlib.h>
#include <iostream>
#include "Module.hpp"
//#include <emscripten.h>
#if defined(EMSCRIPTEN) && defined(_WIN64)
#include "NetworkManager.hpp"
#endif
#include "Utils.hpp"
#include "Vector2.hpp"
#include "Engine.hpp"
#include "TimeModule.hpp"
#include "InputModule.hpp"
#include "EntityManagementSystemModule.hpp"
#include <SDL2/SDL.h>
//#include "RendererModule.hpp"
#include "RendererOpenGL2D.hpp"
#include "FrameRateRegulatorModule.hpp"
#include "FrameRateCounterModule.hpp"
#include "Sprite.hpp"
#include "Camera3D.hpp"
#include "DirectionalLight.hpp"-
#include "Game.hpp"
//#include "ImGuiModule.hpp"

using namespace PrEngine;

void main_loop(void* game_engine);

bool is_running = true;

int main(int argc, char * argv[]) 
{        
        

        // create a new game engine instance
        PrEngine::Engine* game_engine = new PrEngine::Engine();
        // add the components
        // render, frame regulator and frame counter should be the last three components updated ( and so added to engine )
        game_engine->add_module( new PrEngine::Time("Time", 0));
        //game_engine->add_module(new PrEngine::Input( "Input", 1));
        game_engine->add_module(new PrEngine::InputManager("Input",1));
        PrEngine::EntityManagementSystem* entity_management_system = 
                        ( PrEngine::EntityManagementSystem*)game_engine->
                                add_module(new PrEngine::EntityManagementSystem("EMS", 2));
        
        PrEngine::FrameRateRegulator* frame_rate_regulator = 
                        (PrEngine::FrameRateRegulator*)game_engine->
                                add_module( new PrEngine::FrameRateRegulator("FrameRateRegulator", 100000));
        PrEngine::FrameRateCounter* frame_rate_counter = 
                        (PrEngine::FrameRateCounter*)game_engine->
                                add_module(new PrEngine::FrameRateCounter("Frame Counter", 100001));
        Game* game = (Game*)(game_engine->add_module(new Game("game module",3)));
        // can't exceed 60fps if vsync is on
        //frame_rate_regulator->set_frame_rate(60);
        frame_rate_regulator->set_uncapped();
        //frame_rate_regulator->set_frame_rate(15);

        #if defined(EMSCRIPTEN) && defined(_WIN64)
        PrEngine::NetworkManager* network_manager = (PrEngine::NetworkManager*) game_engine->
                                add_module(new PrEngine::NetworkManager("Network manager", 5));
        #endif
        
        Entity* camera_ent = new Entity();
        Transform3D* camera_transform = new Transform3D();
        //Camera* camera = new Camera(16, 9, 0.1f, 100.f, 45.f, *camera_transform);
        Camera* camera = new Camera(-8, 8, -4.5f, 4.5f, -10, 10, *camera_transform);
        //camera->projection_type = ORTHOGRAPHIC;
        camera->transform.set_position(0.f, 1.f, -6.f);
        camera_ent->add_componenet(camera_transform);
        camera_ent->add_componenet(camera);
        entity_management_system->assign_id_and_store_entity(*camera_ent);


        PrEngine::RendererOpenGL2D* renderer = (PrEngine::RendererOpenGL2D*) game_engine->
                                                add_module(new PrEngine::RendererOpenGL2D(1280,720,"PrEngine"));
        renderer->set_vsync(true);
        PrEngine::GameController* gc = ((PrEngine::InputManager*)game_engine->get_module("Input"))->get_gamecontroller(0);
        PrEngine::Keyboard* kb =  &((PrEngine::InputManager*)game_engine->get_module("Input"))->keyboard;
        PrEngine::Mouse* mouse =  &((PrEngine::InputManager*)game_engine->get_module("Input"))->mouse;

        //PrEngine::TextureSlicingParameters slicing_param_1(0,0,2560,1600,0,0);
        if(gc == nullptr)
                PrEngine::LOG( PrEngine::LOGTYPE_ERROR,"Didn't return a valid gamecontroller");

        LOG(LOGTYPE_GENERAL, std::string( (const char*)(glGetString(GL_VERSION))));//,",  ",std::string( (const char*)(glGetString(GL_EXTENSIONS)) ));


        Entity* light_ent = new Entity();
        Transform3D* light_tr = new Transform3D();
        light_tr->set_rotation(Vector3<float>(0,0,90));
        DirectionalLight* light = new DirectionalLight(0.5f, 0.3f);
        light_ent->add_componenet(light);
        light_ent->add_componenet(light_tr);
        entity_management_system->assign_id_and_store_entity(*light_ent);


        Transform3D* tim_transform = new Transform3D();
        tim_transform->set_position(0,0,0);
        tim_transform->set_scale(1,1,1);
        Graphics* tim_graphics= renderer->generate_graphics_sprite(get_resource_path("Braid"+PATH_SEP+"tim0.png"), "sprite_mat");
        tim_graphics->model = &(tim_transform->get_transformation());
        tim_graphics->normal = &(tim_transform->get_rotation_transformation());
        Entity* tim_entity = new Entity();
        tim_entity->add_componenet(tim_transform);
        tim_entity->add_componenet(tim_graphics);
        entity_management_system->assign_id_and_store_entity(*tim_entity);



        //graphics_plane->elements[0].material->environment_map_texture = (TextureCubeMap*)cube_map_grpahics->elements[0].material->diffuse_texture;

        //TextureCubeMap* cube_map = new TextureCubeMap(paths);
        //std::cout<<graphics->normal->data[0]<<std::endl;
        /*PrEngine::Player* player = new PrEngine::Player(gc);
        player->keyboard = kb;
        player->mouse = mouse;
        player->main_camera = camera;
        entity_management_system->assign_id_and_store_entity(*player);
        */
        //std::cout<<sprite->graphics.layer<<","<<sprite_cube->graphics.layer<<","<<sprite_cube_2->graphics.layer<<std::endl;
        game_engine->start();
        
        //PrEngine::show_file_structure( PrEngine::get_resource_path(""),"");

        #ifndef EMSCRIPTEN
                game_engine->update();
                game_engine->end();
                delete game_engine;
        #else
                emscripten_set_main_loop_arg(main_loop, game_engine, 0, 1);
        #endif
        
        return 0;
}


void main_loop(void* game_engine)
{
        ((PrEngine::Engine*)game_engine)->update();
}
