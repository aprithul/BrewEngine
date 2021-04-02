//
//  Component.cpp
//  Game Engine
//
//  Created by Aniruddha Prithul on 7/23/17.
//  Copyright © 2017 Aniruddha Prithul. All rights reserved.
//

#include "Module.hpp"

namespace PrEngine
{
    
    Module::Module(std::string name, Int_32 priority)
    {
        this->priority = priority;
        this->name = name;
        this->is_active = true;
    }
    
    Module::~Module()
    {
        
    }

}
