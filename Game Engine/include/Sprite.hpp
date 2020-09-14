#ifndef SPRITE_HPP
#define SPRITE_HPP
#include "Entity.hpp"
#include "TimeModule.hpp"
#include "Transform3D.hpp"
#include <string>
#include "Graphics.hpp"

namespace PrEngine
{

    class Sprite : public Component
    {

        public:
            Sprite(int order, Graphics& graphics);
            ~Sprite();

            int order;
            Graphics& graphics;
    };

}

#endif