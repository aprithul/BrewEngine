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

		
}

void Game::end()
{

}
