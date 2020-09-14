#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "Entity.hpp"
#include "InputModule.hpp"
#include "Camera.hpp"
#include "Transform.hpp"
#include "Vector2.hpp"

namespace PrEngine
{
    class Player : public Entity
    {
        public:
            Player(GameController* game_controller);
            Player(Keyboard* game_controller);
            ~Player();

            void awake() override;
            void start() override;
            void update() override;
            void end() override;

            Transform transform;
            int id;
            GameController* game_controller;
            Keyboard* keyboard;
            Mouse* mouse;
            Camera* main_camera;

    };
}


#endif