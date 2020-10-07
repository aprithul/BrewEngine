#include "Sprite.hpp"
#include "EntityManagementSystemModule.hpp"
#include "RendererOpenGL2D.hpp"

namespace PrEngine
{
	Sprite::Sprite():Component(COMP_SPRITE)
	{
		order = 0;
	}

    Sprite::Sprite(Int_32 order):Component(COMP_SPRITE)
    {
		this->order = order;
	}

	void Sprite::start()
	{
	}

    Sprite::~Sprite()
    {

    }

	void Sprite::add_to_renderer(RendererOpenGL2D* renderer)
	{
		//SpriteLayer* sprite_layer = (SpriteLayer*)renderer->get_layer("Sprite");
		//sprite_layer->sprite_list.push_back(this);
	}

    std::string Sprite::to_string()
    {
		//for(Int_32 i=0; i<)
		std::string text = std::to_string(COMP_SPRITE)+","+std::to_string(order);
		
		return text;

    }

}
