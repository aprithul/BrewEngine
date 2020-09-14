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
#include "RendererOpenGL.hpp"
#include "FrameRateRegulatorModule.hpp"
#include "FrameRateCounterModule.hpp"
#include "Sprite.hpp"
#include "Camera3D.hpp"
#include "DirectionalLight.hpp"
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
        Camera* camera = new Camera(16, 9, 0.1f, 100.f, 45.f, *camera_transform);
        camera->projection_type = PERSPECTIVE;
        camera->transform.set_position(0.f, 1.f, -6.f);

        camera_ent->add_componenet(camera_transform);
        camera_ent->add_componenet(camera);
        entity_management_system->assign_id_and_store_entity(*camera_ent);
        PrEngine::RendererOpenGL* renderer = (PrEngine::RendererOpenGL*) game_engine->
                                                add_module(new PrEngine::RendererOpenGL(1600,900,"PrEngine"));
        renderer->set_vsync(true);
        PrEngine::GameController* gc = ((PrEngine::InputManager*)game_engine->get_module("Input"))->get_gamecontroller(0);
        PrEngine::Keyboard* kb =  &((PrEngine::InputManager*)game_engine->get_module("Input"))->keyboard;
        PrEngine::Mouse* mouse =  &((PrEngine::InputManager*)game_engine->get_module("Input"))->mouse;

        //PrEngine::TextureSlicingParameters slicing_param_1(0,0,2560,1600,0,0);
        
        if(gc == nullptr)
                PrEngine::LOG( PrEngine::LOGTYPE_ERROR,"Didn't return a valid gamecontroller");

        LOG(LOGTYPE_GENERAL, std::string( (const char*)(glGetString(GL_VERSION))));//,",  ",std::string( (const char*)(glGetString(GL_EXTENSIONS)) ));

        
        std::vector<std::string> paths;
        std::string right = get_resource_path( std::string("skybox"+PATH_SEP+"right.jpg"));
        std::string left = get_resource_path( std::string("skybox"+PATH_SEP+"left.jpg"));
        std::string top = get_resource_path( std::string("skybox"+PATH_SEP+"top.jpg"));
        std::string bottom = get_resource_path( std::string("skybox"+PATH_SEP+"bottom.jpg"));
        std::string back = get_resource_path( std::string("skybox"+PATH_SEP+"back.jpg"));
        std::string front = get_resource_path( std::string("skybox"+PATH_SEP+"front.jpg"));
        paths.push_back(right);
        paths.push_back(left);
        paths.push_back(top);
        paths.push_back(bottom);
        paths.push_back(front);
        paths.push_back(back);

        
        Graphics* cube_map_grpahics = renderer->generate_graphics_skybox(paths, "sky_mat");
        Transform3D* skybox_transform = new Transform3D();
        skybox_transform->set_scale(100,100,100);
        cube_map_grpahics->model= &(skybox_transform->get_transformation());
        cube_map_grpahics->normal= &skybox_transform->get_rotation_transformation();
        Entity* skybox = new Entity();
        skybox->add_componenet(skybox_transform);
        skybox->add_componenet(cube_map_grpahics);
        entity_management_system->assign_id_and_store_entity(*skybox);
                

        
        /*Graphics* graphics_plane = renderer->generate_graphics(get_resource_path("").c_str(), get_resource_path(std::string("teapot.obj")).c_str(), 
                get_resource_path(std::string("stonetile.png")).c_str(), "chest.mat");//, (TextureCubeMap*)(cube_map_grpahics->elements[0].material->diffuse_texture));
        Entity* floor = new Entity("floor");
        Transform3D* floor_transform = new Transform3D();
        floor_transform->set_scale(Vector3<float>(1.f,1.f,1.f));
        floor_transform->set_position(Vector3<float>(0.f, -5.0f,0.f));
        graphics_plane->models.push_back( &(floor_transform->get_transformation()));
        graphics_plane->normals.push_back(&floor_transform->get_rotation_transformation());

        floor->add_componenet(floor_transform);
        floor->add_componenet(graphics_plane);
        entity_management_system->assign_id_and_store_entity(*floor);
        ((GuiLayer*)renderer->get_layer("GUI"))->panning =
                &(graphics_plane->elements.begin()->material->panning);
        ((GuiLayer*)renderer->get_layer("GUI"))->tiling =
                &(graphics_plane->elements.begin()->material->tiling);*/
        

        Graphics* teapot_graphic_2 = renderer->generate_graphics(get_resource_path(""), get_resource_path(std::string("TreasureChest"+PATH_SEP+"treasure_chest.obj")), 
        get_resource_path(std::string("TreasureChest"+PATH_SEP+"Treasurechest_DIFF.png")), "stone1.mat");

        Entity* teapot_ent_2 = new Entity();
        Transform3D* teapot_tr_2 = new Transform3D();
        teapot_tr_2->set_position(2.f,0.5f,1.f);
        teapot_tr_2->set_scale(0.01f,0.01f,0.01f);
        teapot_graphic_2->model = &(teapot_tr_2->get_transformation());
        teapot_graphic_2->normal = &(teapot_tr_2->get_rotation_transformation());

        teapot_ent_2->add_componenet(teapot_tr_2);
        teapot_ent_2->add_componenet(teapot_graphic_2);

        entity_management_system->assign_id_and_store_entity(*teapot_ent_2);


        Graphics* teapot_graphic = renderer->generate_graphics(get_resource_path(""), get_resource_path(std::string("teapot.obj")), 
        "", "stone2.mat");

        Entity* teapot_ent = new Entity();
        Transform3D* teapot_tr = new Transform3D();
        teapot_tr->set_position(Vector3<float>(-4.f,0.5f,1.f));
        teapot_tr->set_scale(0.05f,0.05f,0.05f);
        teapot_graphic->model = &(teapot_tr->get_transformation());
        teapot_graphic->normal = &(teapot_tr->get_rotation_transformation());

        teapot_ent->add_componenet(teapot_tr);
        teapot_ent->add_componenet(teapot_graphic);

        entity_management_system->assign_id_and_store_entity(*teapot_ent);



        Graphics* graphics1 = renderer->generate_graphics(get_resource_path("TreasureChest"), get_resource_path(std::string("plane.obj")), 
        get_resource_path(std::string("stonetile.png")), "stone4.mat");

        Entity* chest1 = new Entity();
        Transform3D* chest_tr1 = new Transform3D();
        chest_tr1->set_position(0,-1,0);
        chest_tr1->set_scale(4,4,4);
        graphics1->model=( &(chest_tr1->get_transformation()));
        graphics1->normal = (&chest_tr1->get_rotation_transformation());

        chest1->add_componenet(chest_tr1);
        chest1->add_componenet(graphics1);

        entity_management_system->assign_id_and_store_entity(*chest1);


        /*Graphics* graphics2 = renderer->generate_graphics(get_resource_path("TreasureChest"), get_resource_path(std::string("TreasureChest"+PATH_SEP+"treasure_chest.obj")), 
        get_resource_path(std::string("TreasureChest"+PATH_SEP+"Treasurechest_DIFF.png")), "chest.mat");

        Entity* chest2 = new Entity();
        Transform3D* chest_tr2 = new Transform3D();
        chest_tr2->set_position(Vector3<float>(2.f,0.5f,-3.8f));
        chest_tr2->set_scale(Vector3<float>{0.01f,0.01f,0.01f});
        graphics2->models.push_back( &(chest_tr2->get_transformation()));
        graphics2->normals.push_back(&chest_tr2->get_rotation_transformation());

        chest1->add_componenet(chest_tr2);
        chest1->add_componenet(graphics2);

        entity_management_system->assign_id_and_store_entity(*chest2);*/




        Entity* light_ent = new Entity();
        Transform3D* light_tr = new Transform3D();
        light_tr->set_rotation(Vector3<float>(0,0,90));
        DirectionalLight* light = new DirectionalLight(0.5f, 0.3f);
        light_ent->add_componenet(light);
        light_ent->add_componenet(light_tr);
        entity_management_system->assign_id_and_store_entity(*light_ent);



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