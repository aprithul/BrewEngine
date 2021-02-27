#include "Game.hpp"
#include "InputModule.hpp"
#include "EntityManagementSystemModule.hpp"
#include "TimeModule.hpp"



Game::Game(std::string name, Int_32 priority):Module(name, priority)
{

}

Game::~Game()
{

}

void Game::start()
{

}

Uint_32 camera_id = {};

void Game::update()
{
	//DirectionalLight light = directional_lights[1];
		
	if (camera_id == 0)
	{
		camera_id = entity_management_system->get_active_camera();
	}
	assert(camera_id != 0);
	Transform3D& camera_transform = get_transform(cameras[camera_id].id_transform);


    Float_32 cam_pan_min_speed = 2.f;
	Float_32 cam_pan_max_speed = 7.f;
	Float_32 cam_pan_speed = cam_pan_min_speed;
	if(input_manager->keyboard.get_key(SDLK_LSHIFT))
		cam_pan_speed = cam_pan_max_speed;

	Vector3<Float_32> pos = camera_transform.position;
	if(input_manager->keyboard.get_key(SDLK_w))
		pos.y = pos.y+(Time::Frame_time*cam_pan_speed);
	if(input_manager->keyboard.get_key(SDLK_s))
		pos.y = pos.y-(Time::Frame_time*cam_pan_speed);
	if(input_manager->keyboard.get_key(SDLK_d))
		pos.x = pos.x +(Time::Frame_time*cam_pan_speed);
	if(input_manager->keyboard.get_key(SDLK_a))
		pos.x = pos.x -(Time::Frame_time*cam_pan_speed);

	//if(input_manager->mouse.get_mouse_button(1))
	//{
	//	Vector2<Float_32> _delta(input_manager->mouse.delta.x,-input_manager->mouse.delta.y);
	//	Vector2<Float_32> _delta_f = (_delta.normalize())*(Float_32)(cam_pan_speed*2*Time::Frame_time);
	//	Vector3<Float_32> _delta3d(_delta_f.x, _delta_f.y, 0);
	//	pos = pos + _delta3d;
	//}
	camera_transform.position = pos;

	//LOG(LOGTYPE_GENERAL, transforms[camera_id].position.to_string());
	if(input_manager->mouse.scroll!=0)
    {
        Float_32 speed = 20.f;
        cameras[camera_id].zoom = clamp<Float_32>(cameras[camera_id].zoom +  + (input_manager->mouse.scroll*Time::Frame_time*speed), 0.1, 1000);
    }

	if (input_manager->keyboard.get_key_down(SDLK_F5))
	{
		entity_management_system->save_scene("scene.graph");
		LOG(LOGTYPE_WARNING, "scene file saved");
	}

	if(input_manager->keyboard.get_key_down(SDLK_ESCAPE))
		input_manager->was_crossed = true;


		
}

void Game::end()
{

}
