#include "DirectionalLight.hpp"

namespace PrEngine
{

	DirectionalLight::DirectionalLight() :Component(COMP_LIGHT)
	{
		this->specular = 0;
		this->ambient = 0;
	}

    DirectionalLight::DirectionalLight(Float_32 _specular, Float_32 _ambient):Component(COMP_LIGHT)
    {
        this->specular = _specular;
        this->ambient = _ambient;
    }

    DirectionalLight::~DirectionalLight()
    {

    }

    std::string DirectionalLight::to_string()
    {
		return std::to_string(COMP_LIGHT) + "," + std::to_string(specular) + "," + std::to_string(ambient);
    }
}
