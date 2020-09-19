#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <string>
#include "Utils.hpp"
#include "Component.hpp"
#include "Serializable.hpp"
namespace PrEngine
{
    class Entity: public Serializable
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
            std::string to_string() override;

            void add_componenet(Component* comp);
            void awake();
            void start();
            void update();
            void end();
    };
}


#endif
