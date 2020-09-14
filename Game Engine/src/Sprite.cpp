#include "Sprite.hpp"

namespace PrEngine
{
    Sprite::Sprite(int order, Graphics& graphics):order(order),graphics(graphics),Component(COMP_SPRITE)
    {

    }

    Sprite::~Sprite()
    {
        delete &graphics;
    }

    

}