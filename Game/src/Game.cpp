#include "Game.hpp"
#include "InputModule.hpp"
#include "EntityManagementSystemModule.hpp"
#include "TimeModule.hpp"
#include "RendererOpenGL2D.hpp"
#include <vector>
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

	Uint_32 _camera = entity_management_system->get_active_camera();
	if (!_camera)
		LOG(LOGTYPE_ERROR, "No active camera in scene");
	//cameras[_camera].zoom = 1.5f;
	
	static const Float_32 cam_pan_min_speed = 10.f;
	static const Float_32 cam_pan_max_speed = 10.f;
	Float_32 cam_pan_speed = cam_pan_min_speed;
	//if (input_manager->keyboard.get_key(SDLK_LSHIFT))
	//	cam_pan_speed = cam_pan_max_speed;
	Point3d pos = transforms[cameras[_camera].id_transform].position;
	pos.y = pos.y + (Time::Frame_time*cam_pan_speed * input_manager->get_gamecontroller(0)->get_axis(SDL_CONTROLLER_AXIS_LEFTY));
	//pos.y = pos.y - (Time::Frame_time*cam_pan_speed * input_manager->get_gamecontroller(0)->get_axis(SDL_CONTROLLER_AXIS_LEFTY));
	pos.x = pos.x + (Time::Frame_time*cam_pan_speed * input_manager->get_gamecontroller(0)->get_axis(SDL_CONTROLLER_AXIS_LEFTX));
	//pos.x = pos.x - (Time::Frame_time*cam_pan_speed * input_manager->get_gamecontroller(0)->get_axis(SDL_CONTROLLER_AXIS_LEFTY));
	transforms[cameras[_camera].id_transform].position = pos;

	if (input_manager->get_gamecontroller(0)->get_axis(SDL_CONTROLLER_AXIS_RIGHTY) != 0)
	{
		Float_32 speed = 20.f;
		cameras[_camera].zoom = clamp<Float_32>(cameras[_camera].zoom + (input_manager->get_gamecontroller(0)->get_axis(SDL_CONTROLLER_AXIS_RIGHTY)*Time::Frame_time*speed), 0.1, 1000);
	}


	/*CollisionShape2D shape_a;
	shape_a.points[0] = { -5,2 };
	shape_a.points[1] = { -5,3 };
	shape_a.points[2] = { -2,2 };
	shape_a.point_count = 3;
	shape_a.type = SHAPE_POLY;
	
	Collider a;
	a.collision_shape = shape_a;
	a.transform_id = 0;


	CollisionShape2D shape_b;
	shape_b.points[0] = { 1,1 };
	shape_b.points[1] = { 1,4 };
	shape_b.points[2] = { 3,1 };
	shape_b.points[3] = { 3,4 };
	shape_b.point_count = 4;

	Collider b;
	b.collision_shape = shape_b;
	b.transform_id = 0;

	if (intersect_GJK(b,a))
		LOG(LOGTYPE_GENERAL, "Collided");*/


	////DirectionalLight light = directional_lights[1];
	//std::vector<Vector2<Float_32>> s1;
	//Vector2<Float_32> p1{ -1, -1 };
	//Vector2<Float_32> p2{ 0, -1 };
	//Vector2<Float_32> p3{ 0, 0 };
	//Vector2<Float_32> p4{ -1, 0 };
	//s1.push_back(p1);
	//s1.push_back(p2);
	//s1.push_back(p3);
	//s1.push_back(p4);

	//Vector4<Float_32> color_red{ 1,0,0,1 };
	//renderer->draw_line(p1, p2, color_red);
	//renderer->draw_line(p2, p3, color_red);
	//renderer->draw_line(p3, p4, color_red);
	//renderer->draw_line(p4, p1, color_red);


	//Vector4<Float_32> color_blue{ 0,0,1,1 };


	//std::vector<Vector2<Float_32>> s2;

	//Vector2<Float_32> v1{ 1, 1 };
	//Vector2<Float_32> v2{ 2, 1 };
	//Vector2<Float_32> v3{ 2, 2 };
	//Vector2<Float_32> v4{ 1, 2 };
	//renderer->draw_line(v1, v2, color_blue);
	//renderer->draw_line(v2, v3, color_blue);
	//renderer->draw_line(v3, v4, color_blue);
	//renderer->draw_line(v4, v1, color_blue);
	//s2.push_back(v1);
	//s2.push_back(v2);
	//s2.push_back(v3);
	//s2.push_back(v4);

	//std::vector<Vector2<Float_32>> s3;

	//Vector4<Float_32> color_white{ 1,1,1,1 };

	//for (auto p : s1)
	//{
	//	for (auto q : s2)
	//	{
	//		Vector2<Float_32> r = p - q;
	//		s3.push_back(r);
	//		if(s3.size() > 0)
	//			renderer->draw_line(s3[s3.size()-2], r, color_white);
	//	}
	//}

}

void Game::end()
{

}
