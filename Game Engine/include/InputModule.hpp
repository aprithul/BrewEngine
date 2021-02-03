//
//  Input.hpp
//  Game Engine
//
//  Created by Aniruddha Prithul on 7/23/17.
//  Copyright © 2017 Aniruddha Prithul. All rights reserved.
//

#ifndef Input_hpp
#define Input_hpp

#include <iostream>
#include <string>
#include <unordered_map>
#include <SDL2/SDL.h>
#include "Module.hpp"
#include "Vector2.hpp"
#include "Utils.hpp"
#include "TimeModule.hpp"
#include "../Vendor/imgui-master/imgui.h"
#include "../Vendor/imgui-master/imgui_impl_sdl.h"

#define MAX_GAMECONTROLLER_COUNT 16
#define MAX_MOUSE_BUTTON_COUNT 10

namespace PrEngine {



    class GameController
    {
        public:
            SDL_GameController* game_controller;            
            SDL_Joystick* joy_stick;
            std::string name;
            Int_32 instance_id;
            Float_32 dead_zone;
            
            GameController();
            GameController(SDL_GameController* game_controller);
            ~GameController();

            Float_32 get_axis(SDL_GameControllerAxis axis);
            Bool_8 get_button(SDL_GameControllerButton action);
            Bool_8 get_button_down(SDL_GameControllerButton action);
            Bool_8 get_button_up(SDL_GameControllerButton action);
            SDL_GameControllerButton find_down_button();
            std::pair<SDL_GameControllerAxis, Float_32> find_max_axis();
            
            SDL_GameControllerAxis axis_binding[SDL_CONTROLLER_AXIS_MAX];
            std::string axis_names[SDL_CONTROLLER_AXIS_MAX];
            Float_32 axis_state[SDL_CONTROLLER_AXIS_MAX]; 

            SDL_GameControllerButton button_binding[SDL_CONTROLLER_BUTTON_MAX];
            std::string button_names[SDL_CONTROLLER_BUTTON_MAX];
            Bool_8 button_state[SDL_CONTROLLER_BUTTON_MAX];
            Bool_8 button_pressed_flags[SDL_CONTROLLER_BUTTON_MAX];
            Bool_8 button_released_flags[SDL_CONTROLLER_BUTTON_MAX];

            

    };

    class KeyboardGameController : public GameController
    {
        public:
            std::string name;
            
            KeyboardGameController();
            ~KeyboardGameController();

            std::unordered_map<SDL_Keycode, std::pair<SDL_GameControllerAxis,Float_32> > axis_binding;
            //std::string axis_names[SDL_CONTROLLER_AXIS_MAX];
            //Float_32 axis_state[SDL_CONTROLLER_AXIS_MAX];

            std::unordered_map<SDL_Keycode, SDL_GameControllerButton> button_binding;
            //Bool_8 button_state[SDL_CONTROLLER_BUTTON_MAX];
            //Bool_8 button_pressed_flags[SDL_CONTROLLER_BUTTON_MAX];
            //Bool_8 button_released_flags[SDL_CONTROLLER_BUTTON_MAX];
    };

    class Mouse
    {
        public:

            Mouse();
            ~Mouse();

            Bool_8 get_mouse_button(Int_32 index);
            Bool_8 get_mouse_button_down(Int_32 index);
            Bool_8 get_mouse_button_up(Int_32 index);
            Int_32 window_id;

            Vector2<Int_32> position;
            Int_32 mb_to_mb_binding[MAX_MOUSE_BUTTON_COUNT];
            SDL_Keycode mb_to_kb_binding[MAX_MOUSE_BUTTON_COUNT];

            Bool_8 button_state[MAX_MOUSE_BUTTON_COUNT];
            Int_32 click_count[MAX_MOUSE_BUTTON_COUNT];
            Bool_8 button_released_flags[MAX_MOUSE_BUTTON_COUNT];
            Bool_8 button_pressed_flags[MAX_MOUSE_BUTTON_COUNT];
            std::unordered_map<SDL_Keycode, Bool_8> kb_down;
            std::unordered_map<SDL_Keycode, Bool_8> kb_up;
            std::unordered_map<SDL_Keycode, Bool_8> kb;
            void map_mb_to_mb(Int_32 from, Int_32 to);
            void map_mb_to_kb(Int_32 from, SDL_Keycode to);
            Vector2<Int_32> delta;
            Int_32 scroll;
    };

    class Keyboard
    {
        public:
            Keyboard();
            ~Keyboard();
            std::unordered_map<SDL_Keycode, SDL_Keycode> key_binding;
            std::unordered_map<SDL_Keycode, std::string> key_names;
            
            std::unordered_map<SDL_Keycode, Bool_8> key_state;
            std::unordered_map<SDL_Keycode, Bool_8> key_pressed_flags;
            std::unordered_map<SDL_Keycode, Bool_8> key_released_flags;

            Bool_8 get_key(SDL_Keycode k);
            Bool_8 get_key_down(SDL_Keycode k);
            Bool_8 get_key_up(SDL_Keycode k);
            SDL_Keycode find_down_key();
    };


    class InputManager : public Module
    {
        public:
    	    static std::string textinput;
    	    static Bool_8 textinput_modified;

            InputManager(std::string name, Int_32 priority);
            ~InputManager();
            Bool_8 was_crossed;
            
            void start() override;
            void update() override;
            void end() override;

            GameController* get_gamecontroller(Int_32 index);
            Keyboard keyboard;
            Mouse mouse;

        private:

            void update_game_controllers();
            GameController game_controllers[MAX_GAMECONTROLLER_COUNT];
            //KeyboardGameController keyboardgc;
            SDL_Event event;
    };

	extern InputManager* input_manager;



}




#endif /* Input_hpp */
