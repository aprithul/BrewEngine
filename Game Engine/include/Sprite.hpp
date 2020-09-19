#ifndef SPRITE_HPP
#define SPRITE_HPP
#include "Entity.hpp"
#include "TimeModule.hpp"
#include "Transform3D.hpp"
#include "Graphics.hpp"
#include "Animator.hpp"
#include <string>

namespace PrEngine
{

    class Sprite : public Component
    {

        public:
            Sprite(int order, Graphics* graphics, Animator* animator);
            ~Sprite();
            std::string to_string() override;
            int order;
            Graphics* graphics;
            Animator* animator;
    };

}

#endif
