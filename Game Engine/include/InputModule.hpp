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
            int instance_id;
            float dead_zone;
            
            GameController();
            GameController(SDL_GameController* game_controller);
            ~GameController();

            float get_axis(SDL_GameControllerAxis axis);
            bool get_button(SDL_GameControllerButton action);
            bool get_button_down(SDL_GameControllerButton action);
            bool get_button_up(SDL_GameControllerButton action);
            SDL_GameControllerButton find_down_button();
            std::pair<SDL_GameControllerAxis, float> find_max_axis();
            
            SDL_GameControllerAxis axis_binding[SDL_CONTROLLER_AXIS_MAX];
            std::string axis_names[SDL_CONTROLLER_AXIS_MAX];
            float axis_state[SDL_CONTROLLER_AXIS_MAX]; 

            SDL_GameControllerButton button_binding[SDL_CONTROLLER_BUTTON_MAX];
            std::string button_names[SDL_CONTROLLER_BUTTON_MAX];
            bool button_state[SDL_CONTROLLER_BUTTON_MAX];
            bool button_pressed_flags[SDL_CONTROLLER_BUTTON_MAX];
            bool button_released_flags[SDL_CONTROLLER_BUTTON_MAX];

            

    };

    class KeyboardGameController : public GameController
    {
        public:
            std::string name;
            
            KeyboardGameController();
            ~KeyboardGameController();

            std::unordered_map<SDL_Keycode, std::pair<SDL_GameControllerAxis,float> > axis_binding;
            //std::string axis_names[SDL_CONTROLLER_AXIS_MAX];
            //float axis_state[SDL_CONTROLLER_AXIS_MAX];

            std::unordered_map<SDL_Keycode, SDL_GameControllerButton> button_binding;
            //bool button_state[SDL_CONTROLLER_BUTTON_MAX];
            //bool button_pressed_flags[SDL_CONTROLLER_BUTTON_MAX];
            //bool button_released_flags[SDL_CONTROLLER_BUTTON_MAX];
    };

    class Mouse
    {
        public:

            Mouse();
            ~Mouse();

            bool get_mouse_button(int index);
            bool get_mouse_button_down(int index);
            bool get_mouse_button_up(int index);
            int window_id;

            Vector2<int> position;
            int mb_to_mb_binding[MAX_MOUSE_BUTTON_COUNT];
            SDL_Keycode mb_to_kb_binding[MAX_MOUSE_BUTTON_COUNT];

            bool button_state[MAX_MOUSE_BUTTON_COUNT];
            int click_count[MAX_MOUSE_BUTTON_COUNT];
            bool button_released_flags[MAX_MOUSE_BUTTON_COUNT];
            bool button_pressed_flags[MAX_MOUSE_BUTTON_COUNT];
            std::unordered_map<SDL_Keycode, bool> kb_down;
            std::unordered_map<SDL_Keycode, bool> kb_up;
            std::unordered_map<SDL_Keycode, bool> kb;
            void map_mb_to_mb(int from, int to);
            void map_mb_to_kb(int from, SDL_Keycode to);
            Vector2<int> delta;
            int scroll;
    };

    class Keyboard
    {
        public:
            Keyboard();
            ~Keyboard();
            std::unordered_map<SDL_Keycode, SDL_Keycode> key_binding;
            std::unordered_map<SDL_Keycode, std::string> key_names;
            
            std::unordered_map<SDL_Keycode, bool> key_state;
            std::unordered_map<SDL_Keycode, bool> key_pressed_flags;
            std::unordered_map<SDL_Keycode, bool> key_released_flags;

            bool get_key(SDL_Keycode k);
            bool get_key_down(SDL_Keycode k);
            bool get_key_up(SDL_Keycode k);
            SDL_Keycode find_down_key();
    };


    class InputManager : public Module
    {
        public:
    		static InputManager* input_manager;
    	    static std::string textinput;
    	    static bool textinput_modified;

            InputManager(std::string name, int priority);
            ~InputManager();
            bool was_crossed;
            
            void start() override;
            void update() override;
            void end() override;

            GameController* get_gamecontroller(int index);
            Keyboard keyboard;
            Mouse mouse;

        private:

            void update_game_controllers();
            GameController game_controllers[MAX_GAMECONTROLLER_COUNT];
            //KeyboardGameController keyboardgc;
            SDL_Event event;
    };


}




#endif /* Input_hpp */
