#include "Sprite.hpp"

namespace PrEngine
{
    Sprite::Sprite(int order, Graphics* graphics, Animator* animator):order(order),graphics(graphics),animator(animator),Component(COMP_SPRITE)
    {

    }

    Sprite::~Sprite()
    {

    }

    std::string Sprite::to_string()
    {
    	return "";
    }

}
