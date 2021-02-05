#include "Component.hpp"

namespace PrEngine
{
     const std::string CompName[COMP_COUNT_MAX] = {
        "UNKNOWN",
        //"TRANSFORM",
        "TRANSFORM_3D",
        "SPRITE",
        "GRAPHICS",
        "LIGHT",
        "CAMERA",
		"ANIMATOR"
    };

    Component::Component(ComponentType type):type(type)
    {
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

}
