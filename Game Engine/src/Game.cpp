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
        /*Entity* entity = entity_management_system->get_entity_with_component(COMP_LIGHT);
        Entity* camera = entity_management_system->get_entity_with_component(COMP_CAMERA);

        if(entity != nullptr)        
        {
            Transform3D* t = (Transform3D*)entity->components[COMP_TRANSFORM_3D];
            if(InputManager::input_manager->keyboard.get_key(SDLK_UP))
            {
                t->set_rotation(t->get_rotation().x+10, 0, 0);
                LOG(LOGTYPE_WARNING, std::to_string(Time::Frame_time));
            }
        }
        else
        {
            LOG(LOGTYPE_ERROR, "Couldn't find entity ", "floor");
        }


        Float_32 cam_pan_min_speed = 2.f;
		Float_32 cam_pan_max_speed = 7.f;
		Float_32 cam_pan_speed = cam_pan_min_speed;
		if(InputManager::input_manager->keyboard.get_key(SDLK_LSHIFT))
			cam_pan_speed = cam_pan_max_speed;

		Transform3D* transform = (Transform3D*)(camera->components[COMP_TRANSFORM_3D]);
		Vector3<Float_32> pos = transform->get_position();
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
		transform->set_position(pos);

        if(InputManager::input_manager->mouse.scroll!=0)
        {
        	Float_32 speed = 4.f;
        	((Camera*)(camera->components[COMP_CAMERA]))->zoom += InputManager::input_manager->mouse.scroll*Time::Frame_time*speed;
        }

		*/
    }

    void Game::end()
    {

    }

}
