#ifndef CUBE_HPP
#define CUBE_HPP

#include "Entity3D.hpp"
#include "Renderer3D.hpp"

namespace PrEngine
{
    class Cube : public Entity3D
    {
        public:
            Cube(Graphics* graphics);
            ~Cube();
            
            void start() override;
            void end() override;
            void update() override;
    };
    
}



#endif