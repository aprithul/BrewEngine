#include "Entity.hpp"

namespace PrEngine
{
    
    void initialize(bool* has_comp, Component** components)
    {
        for(int i=0; i<COMP_COUNT_MAX; i++)
        {
            has_comp[i] = false;
            components[i] = nullptr;
        }
    }
    ///////////////////////////////


    Entity::Entity()
    {
        id = -1;
        initialize(has_component, components);
    }

    Entity::Entity(std::string name)
    {
        id = -1;        
        this->name = name;
        initialize(has_component, components);
    }
    
    Entity::~Entity()
    {
        LOG(LOGTYPE_GENERAL, "Deleting entity (",std::to_string(id),") Components");
        for(int i=0; i<COMP_COUNT_MAX; i++)
        {
            if(components[i] != nullptr){
                LOG(LOGTYPE_GENERAL, "DELETING : ", CompName[components[i]->type]);
                delete components[i];
                LOG(LOGTYPE_GENERAL, "Deleted : ");
            }
        }
    }

    void Entity::awake()
    {
        for(int i=0; i<COMP_COUNT_MAX; i++)
        {
            if(has_component[i]){
                components[i]->awake();
            }
        }
    }

    void Entity::start()
    {
        for(int i=0; i<COMP_COUNT_MAX; i++)
        {
            if(has_component[i])
            {
                components[i]->start();
            }

        }
    }

    void Entity::update()
    {
        for(int i=0; i<COMP_COUNT_MAX; i++)
        {
            if(has_component[i])
                components[i]->update();
        }        
    }

    void Entity::end()
    {
        for(int i=0; i<COMP_COUNT_MAX; i++)
        {
            if(has_component[i])
                components[i]->end();
        }        
    }

    void Entity::add_componenet(Component* component)
    {
        components[component->type] = component;
        has_component[component->type]=true;
		component->set_entity_id(this->id);
    }

    
    std::string Entity::to_string()
    {
    	std::string text = "";
    	for(int i=0; i<COMP_COUNT_MAX; i++)
		{
			if(has_component[i])
			{
				text += components[i]->to_string()+"\n";
			}
		}
    	text+="~\n";
    	return text;
    }

}
