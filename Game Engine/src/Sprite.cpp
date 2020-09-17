#include "Sprite.hpp"

namespace PrEngine
{
    Sprite::Sprite(int order, Graphics& graphics):order(order),graphics(graphics),animator(graphics.elements.back().material),Component(COMP_SPRITE)
    {
    	//animator.set_frame_rate(10);

    }

    Sprite::~Sprite()
    {
        delete &graphics;
    }

    

}
