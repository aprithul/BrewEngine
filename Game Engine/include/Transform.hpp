#ifndef TRANSFORM_HPP
#define TRANSFORM_HPP
#include "Vector2.hpp"
#include "Logger.hpp"
#include "Component.hpp"
#include <string>

namespace PrEngine{
    
    class Transform : public Component
    {
        public:
            Vector2<float> position;
            float angle;
            Vector2<float> scale;
            
            Transform();
            Transform(Vector2<float> position, float angle, Vector2<float> scale);
            ~Transform();
            Vector2<float> translate(const Vector2<float>& translation);
            void rotate(float amount);
    
    };
}
#endif