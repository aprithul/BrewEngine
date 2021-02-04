#ifndef GAME_HPP
#define GAME_HPP


#include "Module.hpp"


using namespace PrEngine;

enum GameMode
{
	GAME_MODE_EDITOR,
	GAME_MODE_PLAY
};

class Game : public Module
{
    public:
        Game(std::string name, Int_32 priority);
        ~Game();

        void start() override;
        void update() override;
        void end() override;

	private:
		GameMode game_mode;

};
#endif
