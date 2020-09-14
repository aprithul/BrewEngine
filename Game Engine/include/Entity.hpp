#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <string>
#include "Transform.hpp"
#include "Utils.hpp"
#include "Component.hpp"

namespace PrEngine
{
    class Entity
    {
        public:
            Entity();
            Entity(std::string name);
            virtual ~Entity();

            long id;
            std::string name;
            bool is_sleeping;
            bool has_component[COMP_COUNT_MAX];
            Component* components[COMP_COUNT_MAX];

            void add_componenet(Component* comp);
            void awake();
            void start();
            void update();
            void end();
    };
}


#endif