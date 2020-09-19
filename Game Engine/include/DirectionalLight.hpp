#ifndef DIRECTIONAL_LIGHT_HPP
#define DIRECTIONAL_LIGHT_HPP

#include "Entity.hpp"
#include "Vector3.hpp"

namespace PrEngine
{
    class DirectionalLight : public Component
    {
        public:
            DirectionalLight(float specular, float ambient);
            ~DirectionalLight() override;
            float specular;
            float ambient;

            void awake() override;
            void start() override;
            void update() override;
            void end() override;
            std::string to_string() override;

    };
}


#endif
