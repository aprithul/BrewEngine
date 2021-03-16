#ifndef COLLIDER_HPP
#define COLLIDER_HPP
#include "Component.hpp"
#include "Vector3.hpp"
namespace PrEngine {
	
	enum Shape2DTypes
	{
		SHAPE_POINT,
		SHAPE_CIRCLE,
		SHAPE_RECT,
		SHAPE_POLY,
		SHAPE_AABB,
		SHAPE_COUNT
	};

	struct CollisionShape2D
	{
		Vector2<Float_32> points[4];
		Shape2DTypes type;
	};


	struct Collider :public Component
	{
		Uint_32 graphic_id;
		Uint_32 transform_id;
		CollisionShape2D collision_shape;
		Collider();
		std::string to_string() override;
	};
}
#endif // !COLLIDER_HPP
