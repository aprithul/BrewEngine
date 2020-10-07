#include "Transform.hpp"
namespace  PrEngine
{
    Transform::Transform() : Component(COMP_TRANSFORM)
    {
        this->position = Vector2<Float_32>(0.f, 0.f);
        this->angle = 0.f;
        this->scale = Vector2<Float_32>(1.f, 1.f);
    }

    Transform::Transform(Vector2<Float_32> position, Float_32 angle, Vector2<Float_32> scale): Component(COMP_TRANSFORM)
    {
        this->position = position;
        this->angle = angle;
        this->scale = scale;
    }

    Transform::~Transform()
    {

    }

    Vector2<Float_32> Transform::translate(const Vector2<Float_32>& translation)
    {
        position = position + translation;
        return this->position;
    }

    void Transform::rotate(Float_32 angle)
    {
        this->angle += angle;
    }

}
