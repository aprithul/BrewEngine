#ifndef GAME_HPP
#define GAME_HPP


#include "Module.hpp"
#include <string>
#include "Transform3D.hpp"
#include "Utils.hpp"
namespace PrEngine
{
    class Game : public Module
    {
        public:
            Game(std::string name, int priority);
            ~Game();

            void start() override;
            void update() override;
            void end() override;
    };
}

#endif