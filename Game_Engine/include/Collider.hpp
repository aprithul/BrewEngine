#ifndef COLLIDER_HPP
#define COLLIDER_HPP
#include <assert.h>
#include "Component.hpp"
#include "Vector3.hpp"
#include "Math.hpp"
#include "Utils.hpp"
#include "Matrix4x4f.hpp"
#include "Transform3D.hpp"
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
		Vector2<Float_32> points[16];
		Shape2DTypes type;
		int point_count;
	};


	struct Collider :public Component
	{
		Uint_32 graphic_id;
		Uint_32 transform_id;
		CollisionShape2D collision_shape;
		Collider();
		std::string to_string() override;
	};
	
	struct Contact
	{
		Vector2<Float_32> depth;
		Uint_32 collider_a;
		Uint_32 collider_b;
	};

	Bool_8 point_in_AABB(Vector2<Float_32> p, Rect<Float_32>& rect);

	Bool_8 intersect_line_line(Vector2<Float_32> l1_p1, Vector2<Float_32> l1_p2, Vector2<Float_32> l2_p1, Vector2<Float_32> l2_p2);

	Bool_8 intersect_AABB_AABB(Rect<Float_32>& rect_a, Rect<Float_32>& rect_b);

	Bool_8 point_in_shape(Vector2<Float_32>* points, Uint_32 count, Vector2<Float_32> p);

	Float_32 solve_line(Vector2<Float_32> v1, Vector2<Float_32> v2, Vector2<Float_32> p);

	Vector2<Float_32> support(Vector2<Float_32> dir, Collider& col_A, Collider& col_B);

	Bool_8 do_simplex(std::vector<Vector2<Float_32>>& simplex, Vector2<Float_32>& dir);

	Bool_8 intersect_GJK(Collider& col_A, Collider& col_B);
	
	Vector2<Float_32> do_EPA(Collider& col_A, Collider& col_B);

	// useful for drawing bounding rects
	Rect<Float_32> points_to_rect(Vector2<Float_32>* points);
	Rect<Float_32> points_to_rect_with_transform(Vector2<Float_32>*  points, Matrix4x4<Float_32>& transformation);
	
	

}
#endif 
