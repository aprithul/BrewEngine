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
		COMP_COLLIDER,
		COMP_SCRIPT,
		COMP_RIGIDBODY_2D,
        COMP_COUNT_MAX,
    };

    struct Component
    {
        Component(ComponentType type);
        virtual ~Component();

		Component(const Component&) = default;
		Component& operator=(const Component&) = default;
		//Component(Component&&) = default;
		//Component& operator=(Component&&) = default;

		virtual void initialize();
        virtual void start();
        virtual void update();
        virtual void end();
        ComponentType type;

		virtual std::string to_string() = 0;
    };
}













#endif
