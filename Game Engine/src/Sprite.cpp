#include "Sprite.hpp"
#include "EntityManagementSystemModule.hpp"
#include "RendererOpenGL2D.hpp"

namespace PrEngine
{
    Sprite::Sprite(int order):Component(COMP_SPRITE)
    {
		this->order = order;
		this->graphics = nullptr;
		this->animator = nullptr;
	}

	void Sprite::start()
	{
		auto _entity = EntityManagementSystem::entity_management_system->get_entity(this->entity_id);
		if (_entity != nullptr)
		{
			if (_entity->has_component[COMP_GRAPHICS])
			{
				this->graphics = (Graphics*)_entity->components[COMP_GRAPHICS];
			}
			else
				LOG(LOGTYPE_ERROR, "couldn't get graphics componenet in sprite");

			if (_entity->has_component[COMP_ANIMATOR])
			{
				this->animator = (Animator*)_entity->components[COMP_ANIMATOR];
			}
			else
				LOG(LOGTYPE_ERROR, "couldn't get animator componenet in sprite");

		}
		else
		{
			LOG(LOGTYPE_ERROR, "Entity with id ", std::to_string(entity_id), " couldn't be found");
		}
	}

    Sprite::~Sprite()
    {

    }

	void Sprite::add_to_renderer(RendererOpenGL2D* renderer)
	{
		SpriteLayer* sprite_layer = (SpriteLayer*)renderer->get_layer("Sprite");
		sprite_layer->sprite_list.push_back(this);
	}

    std::string Sprite::to_string()
    {
		//for(int i=0; i<)
		std::string text = std::to_string(COMP_SPRITE)+","+std::to_string(order);
		
		return text;

    }

}
