#ifndef COLLIDER_HPP
#define COLLIDER_HPP
#include <assert.h>
#include "Component.hpp"
#include "Vec.hpp"
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
		Vec2f points[16];
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
		Vec2f depth;
		Uint_32 collider_a;
		Uint_32 collider_b;
	};

	Bool_8 point_in_AABB(Vec2f p, Rect<Float_32>& rect);

	Bool_8 intersect_line_line(Vec2f l1_p1, Vec2f l1_p2, Vec2f l2_p1, Vec2f l2_p2);

	Bool_8 intersect_AABB_AABB(Rect<Float_32>& rect_a, Rect<Float_32>& rect_b);

	Bool_8 point_in_shape(Vec2f* points, Uint_32 count, Vec2f p);

	Float_32 solve_line(Vec2f v1, Vec2f v2, Vec2f p);

	Vec2f support(Vec2f dir, Collider& col_A, Collider& col_B);

	Bool_8 do_simplex(std::vector<Vec2f>& simplex, Vec2f& dir);

	Bool_8 intersect_GJK(Collider& col_A, Collider& col_B);
	
	Vec2f do_EPA(Collider& col_A, Collider& col_B);

	// useful for drawing bounding rects
	Rect<Float_32> points_to_rect(Vec2f* points);
	Rect<Float_32> points_to_rect_with_transform(Vec2f*  points, Mat4x4& transformation);
	
	

}
#endif 
