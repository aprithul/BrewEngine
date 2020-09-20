#include "DirectionalLight.hpp"

namespace PrEngine
{
    DirectionalLight::DirectionalLight(float _specular, float _ambient):Component(COMP_LIGHT)
    {
        this->specular = _specular;
        this->ambient = _ambient;
    }

    DirectionalLight::~DirectionalLight()
    {

    }

    void DirectionalLight::awake()
    {
    }

    void DirectionalLight::start()
    {

    }

    void DirectionalLight::update()
    {

    }


    void DirectionalLight::end()
    {

    }

    std::string DirectionalLight::to_string()
    {
    	return std::to_string(COMP_LIGHT);
    }
}
