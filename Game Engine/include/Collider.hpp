#ifndef COLLIDER_HPP
#define COLLIDER_HPP
#include "Component.hpp"
#include "Vector3.hpp"
#include "Math.hpp"
#include "Utils.hpp"
#include "Matrix4x4f.hpp"

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
	
	struct Contact
	{
		Uint_32 collider_a;
		Uint_32 collider_b;
	};

	inline Bool_8 point_in_AABB(Vector2<Float_32> p, Rect<Float_32>& rect)
	{
		if (p.x > rect.x && p.x < rect.x + rect.w
			&& p.y > rect.y && p.y < rect.y + rect.h)
			return true;
		else
			return false;
	}

	inline Bool_8 intersect_line_line(Vector2<Float_32> l1_p1, Vector2<Float_32> l1_p2, Vector2<Float_32> l2_p1, Vector2<Float_32> l2_p2)
	{
		// line equation = (y1 - y2)*x + (x2 - x1)*y + (x1*y2 - x2*y1) = 0 
		Float_32 l1_a = l1_p1.y - l1_p2.y;
		Float_32 l1_b = l1_p2.x - l1_p1.x;
		Float_32 l1_c = l1_p1.x * l1_p2.y - l1_p2.x*l1_p1.y;

		// solve line 1 for the two points in line 2
		Float_32 d1 = l1_a * l2_p1.x + l1_b * l2_p1.y + l1_c;
		Float_32 d2 = l1_a * l2_p2.x + l1_b * l2_p2.y + l1_c;

		// if both  points fall in one side of the first line, they couldn't have intersected
		if ((d1 < 0 && d2 < 0) || (d1 > 0 && d2 > 0))
			return false;

		// solve line 2 for for two points in line 1
		Float_32 l2_a = l2_p1.y - l2_p2.y;
		Float_32 l2_b = l2_p2.x - l2_p1.x;
		Float_32 l2_c = l2_p1.x * l2_p2.y - l2_p2.x*l2_p1.y;
		d1 = l2_a * l1_p1.x + l2_b * l1_p1.y + l2_c;
		d2 = l2_a * l1_p2.x + l2_b * l1_p2.y + l2_c;

		if ((d1 < 0 && d2 < 0) || (d1 > 0 && d2 > 0))
			return false;

		// determinant is ~0, so colinear 
		Float_32 derivative = l1_a * l2_b - l1_b * l2_a;
		if (derivative < EPSILON && derivative > -EPSILON)
			return false;

		return true;
	}

	inline Bool_8 intersect_AABB_AABB(Rect<Float_32>& rect_a, Rect<Float_32>& rect_b)
	{
		Float_32 x_min = rect_a.x < rect_b.x ? rect_a.x : rect_b.x;
		Float_32 x_max = rect_a.x+rect_a.w > rect_b.x + rect_b.w ? rect_a.x + rect_a.w : rect_b.x + rect_b.w;
		Float_32 y_min = rect_a.y < rect_b.y ? rect_a.y : rect_b.y;
		Float_32 y_max = rect_a.y + rect_a.h > rect_b.y + rect_b.h ? rect_a.y + rect_a.h : rect_b.y + rect_b.h;

		if ((x_max - x_min < rect_a.w + rect_b.w) && (y_max - y_min < rect_a.h + rect_b.h))
			return true;
		
		return false;
	}

	inline Bool_8 point_in_shape(Vector2<Float_32>* points, Uint_32 count, Vector2<Float_32> p)
	{
		// simplex in 2d is triangle
		if (count < 3)
			return false;

		Float_32 x_min = points[0].x;
		Float_32 x_max = points[0].x;
		Float_32 y_min = points[0].y;
		Float_32 y_max = points[0].y;

		for (int _i = 1; _i < count; _i++)
		{
			if (points[_i].x < x_min)
				x_min = points[_i].x;
			if (points[_i].x > x_max)
				x_max = points[_i].x;
			if (points[_i].y < y_min)
				y_min = points[_i].y;
			if (points[_i].y > y_max)
				y_max = points[_i].y;
		}

		// Can't be inside shape if not in AABB
		if (!point_in_AABB(p, Rect<Float_32>{x_min, y_min, (x_max - x_min), (y_max - y_min)}))
			return false;

		// farther test ( crossing number algorithm )

		// two points of line from outiside of bounding box to point
		Vector2<Float_32> l1_p1{ x_min - 0.1f, y_min };
		Vector2<Float_32> l1_p2 = p;

		Int_32 intersection_count = 0;
		// check intersectin with every edge of shape
		for (int _i = 0; _i < count; _i++)
		{
			// two points of line 2 ( an edge of shape)
			Vector2<Float_32> l2_p1 = points[_i];
			Vector2<Float_32> l2_p2 = points[(_i + 1) % count];

			Bool_8 intersection_res = intersect_line_line(l1_p1, l1_p2, l2_p1, l2_p2);
			if (intersection_res)
				intersection_count++;
		}

		// if odd, then inside
		if ((intersection_count & 1))	// odd number will have 1 as last digit
			return true;

		return false;

	}

	inline Rect<Float_32> points_to_rect(Vector2<Float_32>* points)
	{
		Float_32 x_min = points[0].x;
		Float_32 x_max = points[0].x;
		Float_32 y_min = points[0].y;
		Float_32 y_max = points[0].y;

		for (int _i = 1; _i < 4; _i++)
		{
			if (points[_i].x < x_min)
				x_min = points[_i].x;
			if (points[_i].x > x_max)
				x_max = points[_i].x;
			if (points[_i].y < y_min)
				y_min = points[_i].y;
			if (points[_i].y > y_max)
				y_max = points[_i].y;
		}

		Rect<Float_32> rect{ x_min,y_min, (x_max - x_min),(y_max - y_min) };
		return rect;
	}

	inline Rect<Float_32> points_to_rect_with_transform(Vector2<Float_32>*  points,  Matrix4x4<Float_32>& transformation)
	{
		Float_32 x_min = std::numeric_limits<Float_32>::infinity();
		Float_32 x_max = -std::numeric_limits<Float_32>::infinity();
		Float_32 y_min = std::numeric_limits<Float_32>::infinity();
		Float_32 y_max = -std::numeric_limits<Float_32>::infinity();

		for (int _i = 1; _i < 4; _i++)
		{
			Vector2<Float_32> p = transformation * points[_i];
			if (p.x < x_min)
				x_min = p.x;
			if (p.x > x_max)
				x_max = p.x;
			if (p.y < y_min)
				y_min = p.y;
			if (p.y > y_max)
				y_max = p.y;
		}

		Rect<Float_32> rect{ x_min,y_min, (x_max - x_min),(y_max - y_min) };
		return rect;
	}
	
}
#endif 
