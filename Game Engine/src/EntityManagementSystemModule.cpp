#include "EntityManagementSystemModule.hpp"

namespace PrEngine
{
    EntityManagementSystem* EntityManagementSystem::entity_management_system;
    long EntityManagementSystem::next_entity_id = 0;
    int EntityManagementSystem::current_max_entity_pos = 0;
    
    int EntityManagementSystem::entity_count = 0;
    
    EntityManagementSystem::EntityManagementSystem(std::string name, int priority) : Module(name, priority)
    {
        //entities = new (*Entity)[MAX_ENTITY_COUNT];
        for(int i=0;i<MAX_ENTITY_COUNT;i++)
            entities[i] = nullptr;
        started = false;
        EntityManagementSystem::entity_management_system = this;
    }

    EntityManagementSystem::~EntityManagementSystem()
    {
        for(int i=0; i<=next_entity_id; i++)
        {
            if(entities[i] != nullptr)
            {
                //LOG(LOGTYPE_GENERAL, std::string("Deleting entity, id: ").append(std::to_string(entities[i]->id)));
                delete entities[i];
            }
        }
    }

    Entity* EntityManagementSystem::get_entity_with_component(ComponentType type)
    {
        for(int i=0;i < next_entity_id; i++)
        {
            if(entities[i]->has_component[type])
                return entities[i];
        }
        return nullptr;
    }    


    Entity* EntityManagementSystem::get_entity(long id)
    {
        std::unordered_map<long,int>::iterator it = id_map.find(id);
        if(it != id_map.end())
        {   
            Entity* _entity = entities[it->second];    
            if(_entity)
            {
                return _entity;
            }
            else
            {
                LOG(LOGTYPE_ERROR, "No entity  with id exists");
            }
        }
        else
        {
            LOG(LOGTYPE_ERROR, "requested entity couldn't be found : ", std::to_string(id));
        }

        return nullptr;
    
    }

    Entity* EntityManagementSystem::get_entity(std::string name)
    {
        for(int i=0;i < entity_count; i++)
        {
            if(entities[i]->name == name)
                return entities[i];
        }
        return nullptr;
    
    }


    bool EntityManagementSystem::delete_entity(long id)
    {
        if(id<=next_entity_id)
        {   
            Entity* _entity = nullptr;
            std::unordered_map<long,int>::iterator it = id_map.find(id);
            if(it != id_map.end())
                _entity = entities[it->second];

            if(_entity!= nullptr)
            {
                
                delete _entity;
                entities[it->second] = nullptr;
                entity_count--;
                released_positions.push(it->second);
                LOG(LOGTYPE_GENERAL, std::string("Entity id: ").append(std::to_string(id)).append(" deleted"));
                return true;
            }
            else
            {
                LOG(LOGTYPE_WARNING, std::string("No entity with id: ").append(std::to_string(id)).append(" exists"));
            }
        }
        else
        {
            LOG(LOGTYPE_WARNING, std::string("No entity with id: ").append(std::to_string(id)).append(" exists"));
        }
        return false;
    }


    Entity* EntityManagementSystem::assign_id_and_store_entity(Entity& entity)
    {
        if(entity.id != -1)
        {
            LOG(LOGTYPE_WARNING, "Entity already present in system, can't add duplicate");
            
            return &entity;       
        }

        // get next id either from freed id queue or create a new id if none available in queue
        int next_pos = -1;
        //LOG(LOGTYPE_GENERAL, "Length:   ",std::to_string(released_ids.size()));

        if(!released_positions.empty())
        {
            next_pos = released_positions.front();
            released_positions.pop();
        }
        else
        {
            next_pos = (current_max_entity_pos++);
        }
        
        
        if(next_entity_id < LONG_MAX)
        {
            next_entity_id++;
        }
        else
        {
            LOG(LOGTYPE_ERROR, "Ran out of unique entity ids");
            return nullptr;

        }

        entity.id = next_entity_id;
        entities[next_pos] = &entity;
        id_map[next_entity_id] = next_pos; 
        entity_count++;

        if(started)
        {
            entity.awake();
            entity.start();
        }

        return &entity;
    }


    void EntityManagementSystem::start()
    {
        LOG(LOGTYPE_GENERAL, "Entity Management System started");
        for(int _i=0; _i<next_entity_id; _i++)
            if(entities[_i] != nullptr)
                entities[_i]->awake();
        LOG(LOGTYPE_GENERAL, "Entity Management callled awake() on all entities");
        
        for(int _i=0; _i<next_entity_id; _i++)
            if(entities[_i] != nullptr)
                entities[_i]->start();
        LOG(LOGTYPE_GENERAL, "Entity Management System called start() on all entities");

        started = true;
    }

    void EntityManagementSystem::update()
    {
        //LOG(LOGTYPE_GENERAL, std::string("Entity count: ").append(std::to_string(entity_count)).append("  max id: ").append(std::to_string(current_max_entity_id).append("  queue_size: ").append(std::to_string(this->released_ids.size()))));
        for(int _i=0; _i<next_entity_id; _i++)
            if(entities[_i] != nullptr)
                entities[_i]->update();
        
        //LOG(LOGTYPE_GENERAL, "Entity Management System called update() on all entities");

    }

    void EntityManagementSystem::end()
    {
        for(int _i=0; _i<next_entity_id; _i++)
            if(entities[_i] != nullptr)
                entities[_i]->end();
        LOG(LOGTYPE_GENERAL, "Entity Management System called end() on all entities");

    }

}
