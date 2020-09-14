#include "Player.hpp"

namespace PrEngine{

    Player::Player(GameController* game_controller):Entity(ENTITY_TYPE_PLAYER)
    {
        this->game_controller = game_controller;
    }

    Player::Player(Keyboard* keyboard):Entity(ENTITY_TYPE_PLAYER)
    {
        this->keyboard = keyboard;
    }

    Player::~Player()
    {

    }

    void Player::awake()
    {

    }

    void Player::start()
    {
    
    }

    void Player::update()
    {
        /*
        if(keyboard->get_key_down(SDLK_ESCAPE))
            std::cout<<"escape pressed"<<std::endl;
                
        if(keyboard->get_key_up(SDLK_ESCAPE))
            std::cout<<"escape released"<<std::endl;

        if(keyboard->get_key(SDLK_c))
            std::cout<<"c held"<<std::endl;
        
        if(keyboard->get_key_down(SDLK_t))
            std::cout<<"t pressed"<<std::endl;

        if(keyboard->get_key_up(SDLK_t))
            std::cout<<"t released"<<std::endl;
        */

        Vector2<float> translation( game_controller->get_axis(SDL_CONTROLLER_AXIS_LEFTX), game_controller->get_axis(SDL_CONTROLLER_AXIS_LEFTY));
        translation = translation * (float)(Time::Frame_time*5.f);
        main_camera->transform.translate(translation);

        main_camera->zoom_in( 1 * game_controller->get_axis(SDL_CONTROLLER_AXIS_TRIGGERLEFT));
        main_camera->zoom_out( 1 * game_controller->get_axis(SDL_CONTROLLER_AXIS_TRIGGERRIGHT));

        if(game_controller->get_button_down(SDL_CONTROLLER_BUTTON_A))
            LOG(LOGTYPE_GENERAL, "A down");
/*
        if(game_controller->get_button_down(SDL_CONTROLLER_BUTTON_A))
            std::cout<<"A down"<<std::endl;
        if(game_controller->get_button_up(SDL_CONTROLLER_BUTTON_A))
            std::cout<<"A up"<<std::endl;
        if(game_controller->get_button(SDL_CONTROLLER_BUTTON_B))
            std::cout<<"B held"<<std::endl;

        if(game_controller->get_button_down(SDL_CONTROLLER_BUTTON_X))
            std::cout<<"X down"<<std::endl;
        if(game_controller->get_button_up(SDL_CONTROLLER_BUTTON_X))
            std::cout<<"X up"<<std::endl;
        if(game_controller->get_button(SDL_CONTROLLER_BUTTON_Y))
            std::cout<<"Y held"<<std::endl;
        //"major bug fixes in input module. Bindign was all wrong. Keyboard emulation with game controller not implemented for now. Also added capability to find what key/axis/button is down"
        SDL_Keycode dk= keyboard->find_down_key();
        if(dk != -1)
        {
            std::cout<< char(dk) <<" down (value : "<<dk<<")"<<std::endl;
        }

        std::pair<SDL_GameControllerAxis,float> a_v = game_controller->find_max_axis();
        if(a_v.first != SDL_CONTROLLER_AXIS_INVALID)
        {
            std::cout<< "Max axis is: "<<(SDL_GameControllerAxis)a_v.first<<" Value: "<<a_v.second<<std::endl;
        }

       SDL_GameControllerButton b =  game_controller->find_down_button();
       if( b != SDL_CONTROLLER_BUTTON_INVALID)
       {
           std::cout<<b<<" is down"<<std::endl;
       }

        if(mouse->get_mouse_button_down(0))
            std::cout<<"mouse 0 down"<<std::endl;
        if(mouse->get_mouse_button_up(0))
            std::cout<<"mouse 0 up"<<std::endl;
        if(mouse->get_mouse_button(0))
            std::cout<<"mouse 0 held"<<std::endl;
        */

    }

    void Player::end()
    {

    }


}