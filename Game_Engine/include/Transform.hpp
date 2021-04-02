#ifndef TRANSFORM_HPP
#define TRANSFORM_HPP
#include "Vector2.hpp"
#include "Logger.hpp"
#include "Component.hpp"
#include <string>

namespace PrEngine{
    
    struct Transform : public Component
    {
        Vector2<Float_32> position;
        Float_32 angle;
        Vector2<Float_32> scale;

        Transform();
        Transform(Vector2<Float_32> position, Float_32 angle, Vector2<Float_32> scale);
        ~Transform();
        Vector2<Float_32> translate(const Vector2<Float_32>& translation);
        void rotate(Float_32 amount);
    };
}
#endif
