#include "Transform.hpp"
namespace  PrEngine
{
    Transform::Transform() : Component(COMP_TRANSFORM)
    {
        this->position = Vec2f(0.f, 0.f);
        this->angle = 0.f;
        this->scale = Vec2f(1.f, 1.f);
    }

    Transform::Transform(Vec2f position, Float_32 angle, Vec2f scale): Component(COMP_TRANSFORM)
    {
        this->position = position;
        this->angle = angle;
        this->scale = scale;
    }

    Transform::~Transform()
    {

    }

    Vec2f Transform::translate(const Vec2f& translation)
    {
        position = position + translation;
        return this->position;
    }

    void Transform::rotate(Float_32 angle)
    {
        this->angle += angle;
    }

}
