//
//  Component.hpp
//  Game Engine
//
//  Created by Aniruddha Prithul on 7/22/17.
//  Copyright © 2017 Aniruddha Prithul. All rights reserved.
//

#ifndef MODULE_HPP
#define MODULE_HPP

#include <string>
#include "Types.hpp"
#define IS_SERVER false

namespace PrEngine {
    
    // every component class extends this
    class Module
    {
    public:
        Module(std::string name, Int_32 priority);
        virtual ~Module();
        virtual void start() = 0;
        virtual void update() = 0;
        virtual void end() = 0;
        Bool_8 is_active;
        std::string name;
        Int_32 priority;
        
    };
}

#endif /* Component_hpp */
