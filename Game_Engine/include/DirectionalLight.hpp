#ifndef DIRECTIONAL_LIGHT_HPP
#define DIRECTIONAL_LIGHT_HPP

#include "Entity.hpp"
#include "Vector3.hpp"

namespace PrEngine
{
    class DirectionalLight : public Component
    {
        public:
			DirectionalLight();
            DirectionalLight(Float_32 specular, Float_32 ambient);
            ~DirectionalLight() override;
            Float_32 specular;
            Float_32 ambient;
			Uint_32 id_transform;

            std::string to_string() override;

    };
}


#endif
