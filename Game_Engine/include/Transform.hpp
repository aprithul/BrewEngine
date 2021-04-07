#ifndef TRANSFORM_HPP
#define TRANSFORM_HPP
#include "Vec.hpp"
#include "Logger.hpp"
#include "Component.hpp"
#include <string>

namespace PrEngine{
    
    struct Transform : public Component
    {
        Vec2f position;
        Float_32 angle;
        Vec2f scale;

        Transform();
        Transform(Vec2f position, Float_32 angle, Vec2f scale);
        ~Transform();
        Vec2f translate(const Vec2f& translation);
        void rotate(Float_32 amount);
    };
}
#endif
