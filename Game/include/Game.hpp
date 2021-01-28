#ifndef GAME_HPP
#define GAME_HPP


#include "Module.hpp"


using namespace PrEngine;

class Game : public Module
{
    public:
        Game(std::string name, Int_32 priority);
        ~Game();

        void start() override;
        void update() override;
        void end() override;

};
#endif
