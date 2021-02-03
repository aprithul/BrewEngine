#ifndef COMPONENET_HPP
#define COMPONENET_HPP

#include <string>
#include <vector>
#include "Serializable.hpp"
#include "Types.hpp"
namespace PrEngine
{

    enum ComponentType
    {
        COMP_UNKNOWN,
        COMP_TRANSFORM,
        COMP_TRANSFORM_3D,
        COMP_SPRITE,
        COMP_GRAPHICS,
        COMP_LIGHT,
        COMP_CAMERA,
		COMP_ANIMATOR,
        COMP_COUNT_MAX,
    };
    extern const std::string CompName[COMP_COUNT_MAX];

    struct Component
    {
        Component(ComponentType type);
        virtual ~Component();
        virtual void awake();
        virtual void start();
        virtual void update();
        virtual void end();
        const ComponentType type;

		virtual std::string to_string() = 0;
    };
}













#endif
