#include "Game.hpp"
#include "InputModule.hpp"
#include "EntityManagementSystemModule.hpp"
#include "TimeModule.hpp"

namespace PrEngine
{
    Game::Game(std::string name, int priority):Module(name, priority)
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
        Entity* entity = entity_management_system->get_entity_with_component(COMP_LIGHT);
        if(entity != nullptr)        
        {
            Transform3D* t = (Transform3D*)entity->components[COMP_TRANSFORM_3D];
            if(input_manager->keyboard.get_key(SDLK_UP))
            {
                t->set_rotation(t->get_rotation().x+10, 0, 0);
                LOG(LOGTYPE_WARNING, std::to_string(Time::Frame_time));
            }
        }
        else
        {
            LOG(LOGTYPE_ERROR, "Couldn't find entity ", "floor");
        }
    }

    void Game::end()
    {

    }

}