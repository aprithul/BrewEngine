#ifndef ENTITY_MANAGEMENT_SYSTEM_HPP
#define ENTITY_MANAGEMENT_SYSTEM_HPP

#include "Module.hpp"
#include "Entity.hpp"
#include "Logger.hpp"
#include <unordered_map>
#include <vector>
#include <queue>
#include <climits>

#define MAX_ENTITY_COUNT 99999

namespace PrEngine
{
    class EntityManagementSystem : public Module
    {
        public:
    		static EntityManagementSystem* entity_management_system;
            EntityManagementSystem(std::string name, int priority);
            ~EntityManagementSystem();

            Entity* get_entity(long id);
            Entity* get_entity(std::string name);
            Entity* get_entity_with_component(ComponentType type);
            bool delete_entity(long id);
            Entity* assign_id_and_store_entity(Entity& entity);

            Entity* entities[MAX_ENTITY_COUNT];
            static int current_max_entity_pos;
            
            void start() override;
            void update() override;
            void end() override;

        private:
            //Entity* entities[MAX_ENTITY_ID];
            std::unordered_map<long, int> id_map;
            std::queue<int> released_positions;
            static long next_entity_id;
            static int entity_count;
            bool started;
    };


}


#endif
