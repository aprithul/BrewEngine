#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <string>
#include "Utils.hpp"
#include "Component.hpp"
#include "Serializable.hpp"
namespace PrEngine
{
    class Entity
    {
        public:
            Entity();
            Uint_32 id;
            std::string to_string();
    };
}

#endif