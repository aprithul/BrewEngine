#include "Game.hpp"
#include "InputModule.hpp"
#include "EntityManagementSystemModule.hpp"
#include "TimeModule.hpp"

namespace PrEngine
{


    Game::Game(std::string name, Int_32 priority):Module(name, priority)
    {

    }

    Game::~Game()
    {

    }

    void Game::start()
    {

    }


    void Game::update()
    {
		DirectionalLight light = directional_lights[0];
		Camera camera = cameras[0];


        Float_32 cam_pan_min_speed = 2.f;
		Float_32 cam_pan_max_speed = 7.f;
		Float_32 cam_pan_speed = cam_pan_min_speed;
		if(InputManager::input_manager->keyboard.get_key(SDLK_LSHIFT))
			cam_pan_speed = cam_pan_max_speed;

		Vector3<Float_32> pos = transforms[camera.id_transform].position;
		if(InputManager::input_manager->keyboard.get_key(SDLK_w))
			pos.y = pos.y+(Time::Frame_time*cam_pan_speed);
		if(InputManager::input_manager->keyboard.get_key(SDLK_s))
			pos.y = pos.y-(Time::Frame_time*cam_pan_speed);
		if(InputManager::input_manager->keyboard.get_key(SDLK_d))
			pos.x = pos.x +(Time::Frame_time*cam_pan_speed);
		if(InputManager::input_manager->keyboard.get_key(SDLK_a))
			pos.x = pos.x -(Time::Frame_time*cam_pan_speed);

		if(InputManager::input_manager->mouse.get_mouse_button(1))
		{
			Vector2<Float_32> _delta(InputManager::input_manager->mouse.delta.x,-InputManager::input_manager->mouse.delta.y);
			Vector2<Float_32> _delta_f = (_delta.normalize())*(Float_32)(cam_pan_speed*2*Time::Frame_time);
			Vector3<Float_32> _delta3d(_delta_f.x, _delta_f.y, 0);
			pos = pos + _delta3d;
		}
		transforms[camera.id_transform].position = pos;

        if(InputManager::input_manager->mouse.scroll!=0)
        {
        	Float_32 speed = 4.f;
        	cameras[0].zoom += InputManager::input_manager->mouse.scroll*Time::Frame_time*speed;
        }
		
    }

    void Game::end()
    {

    }

}
