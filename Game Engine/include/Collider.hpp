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
		// at least 3 points needed to contain another point
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
		// check intersections with every edge of shape
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

	inline Float_32 solve_line(Vector2<Float_32> v1, Vector2<Float_32> v2, Vector2<Float_32> p)
	{
		Float_32 A = v1.y - v2.y;
		Float_32 B = v1.x - v2.x;
		Float_32 C = v1.x * v2.y - v2.x*v1.y;

		// solve line 1 for the two points in line 2
		return A * p.x + B * p.y + C;

	}

	inline Vector2<Float_32> support(Vector2<Float_32> dir, Collider& col_A, Collider& col_B)
	{
		//assert(col_A.transform_id && col_B.transform_id);

		// find max point in A along dir
		Vector2<Float_32> A_max;
		Float_32 max_proj = -std::numeric_limits<Float_32>::infinity();
		for (Uint_32 i = 0; i < col_A.collision_shape.point_count; i++)
		{
			Vector2<Float_32> point = (transforms[col_A.transform_id].transformation * col_A.collision_shape.points[i]);
			Float_32 proj = dir * point;
			if (proj > max_proj)
			{
				max_proj = proj;
				A_max = point;
			}
		}

		// find max point in B along  (-dir)
		Vector2<Float_32> B_max;
		max_proj = -std::numeric_limits<Float_32>::infinity();
		for (Uint_32 i = 0; i < col_B.collision_shape.point_count; i++)
		{
			Vector2<Float_32> point = (transforms[col_B.transform_id].transformation * col_B.collision_shape.points[i]);
			Float_32 proj = -dir * point;
			if (proj > max_proj)
			{
				max_proj = proj;
				B_max = point;
			}
		}


		return A_max - B_max;
	}

	inline Bool_8 do_simplex(std::vector<Vector2<Float_32>>& simplex, Vector2<Float_32>& dir)
	{
		static Vector2<Float_32> origin{ 0,0 };
		if (simplex.size() == 3)	//we got a triangle. so check if it contains origin
		{
			// for side A of triangle
			Vector3<Float_32> vec1 = simplex[2] - simplex[1];
			Vector3<Float_32> vec2 = -simplex[1];
			Vector3<Float_32> perp = (vec1^vec2) ^ vec1;

			Float_32 _dotA = perp * (simplex[0]-simplex[2]);

			if (_dotA < 0) // in A region
			{
				dir = perp;
				simplex.erase(simplex.begin());
				return false;
			}

			//------------------------------------------------

			// for side B of triangle
			vec1 = simplex[2] - simplex[0];
			vec2 = -simplex[0];
			perp = (vec1^vec2) ^ vec1;

			Float_32 _dotB = perp * (simplex[1]- simplex[2]);

			if (_dotB < 0) // in B region
			{
				dir = perp;
				simplex.erase(simplex.begin()+1);
				return false;
			}

			///
			//else inside triangle
			return true;



		}
		else if (simplex.size() == 2)
		{
			Vector3<Float_32> vec1 = -simplex[1];
			Vector3<Float_32> vec2 = simplex[0] - simplex[1];
			//Float_32 _dot = vec1 * vec2;
			//if (_dot > 0)	// new direction is normal to vec2 in the direction of origin
			//{
				Vector3<Float_32> perp = (vec2^vec1) ^ vec2;
				dir = perp.normalize();
				return false;
			//}
			//else
			//{
			//	dir = vec1;
			//	simplex.clear();
			//	simplex.push_back(-vec1);	// only keep the last point
			//}

		}
	}

	inline Bool_8 intersect_GJK(Collider& col_A, Collider& col_B)
	{
		static Vector2<Float_32> origin{ 0,0 };

		std::vector<Vector2<Float_32>> simplex;
		Vector2<Float_32> dir{ 1,0 };
		Vector2<Float_32> p = support(dir, col_A, col_B);
		simplex.push_back(p);
		
		dir = - p;

		do {
			p = support(dir, col_A, col_B);

			Float_32 _dot = dir * p;
			if (_dot < 0) // extreme point in direction of origin doesn't reach origin. so can't intersect
				return false;
			simplex.push_back(p);
			if (do_simplex(simplex, dir))
				return true;

		} while (true);

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
