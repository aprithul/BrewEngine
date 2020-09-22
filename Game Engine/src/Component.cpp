#include "Component.hpp"

namespace PrEngine
{

     const std::string CompName[COMP_COUNT_MAX] = {
        "UNKNOWN",
        "TRANSFORM",
        "TRANSFORM_3D",
        "SPRITE",
        "GRAPHICS",
        "LIGHT",
        "CAMERA",
		"ANIMATOR"
    };

    Component::Component(ComponentType type):type(type)
    {
		this->entity_id = -1;
		this->is_entity_id_set = false;
    }
    
    Component::~Component()
    {
    }

    void Component::awake()
    {

    }

    void Component::start()
    {

    }

    void Component::update()
    {

    }

    void Component::end()
    {

    }

	void Component::set_entity_id(int entity_id)
	{
		if(!is_entity_id_set)
		{
			this->entity_id = entity_id;
			is_entity_id_set = !is_entity_id_set;
		}
	}

}
