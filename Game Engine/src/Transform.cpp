#include "Transform.hpp"
namespace  PrEngine
{
    Transform::Transform() : Component(COMP_TRANSFORM)
    {
        this->position = Vector2<float>(0.f, 0.f);
        this->angle = 0.f;
        this->scale = Vector2<float>(1.f, 1.f);
    }

    Transform::Transform(Vector2<float> position, float angle, Vector2<float> scale): Component(COMP_TRANSFORM)
    {
        this->position = position;
        this->angle = angle;
        this->scale = scale;
    }

    Transform::~Transform()
    {

    }

    Vector2<float> Transform::translate(const Vector2<float>& translation)
    {
        position = position + translation;
        return this->position;
    }

    void Transform::rotate(float angle)
    {
        this->angle += angle;
    }
}