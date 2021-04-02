#include "Collider.hpp"
#include "PhysicsModule.hpp"
#include "EntityManagementSystemModule.hpp"
#include <time.h>
namespace PrEngine
{
	Collider::Collider() :Component(COMP_COLLIDER)
	{
		graphic_id = 0;
		transform_id= 0;
	}


	std::string Collider::to_string()
	{
		std::string _s = std::to_string(COMP_COLLIDER)+","+ std::to_string(collision_shape.type);
		for (int _i = 0; _i < collision_shape.point_count; _i++)
		{
			_s += ","+collision_shape.points[_i].to_string();
		}
		return _s;
	}


	Bool_8 point_in_AABB(Vector2<Float_32> p, Rect<Float_32>& rect)
	{
		if (p.x > rect.x && p.x < rect.x + rect.w
			&& p.y > rect.y && p.y < rect.y + rect.h)
			return true;
		else
			return false;
	}

	std::vector<Vector2<Float_32>> simplex;

	Bool_8 intersect_line_line(Vector2<Float_32> l1_p1, Vector2<Float_32> l1_p2, Vector2<Float_32> l2_p1, Vector2<Float_32> l2_p2)
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

	Bool_8 intersect_AABB_AABB(Rect<Float_32>& rect_a, Rect<Float_32>& rect_b)
	{
		Float_32 x_min = rect_a.x < rect_b.x ? rect_a.x : rect_b.x;
		Float_32 x_max = rect_a.x + rect_a.w > rect_b.x + rect_b.w ? rect_a.x + rect_a.w : rect_b.x + rect_b.w;
		Float_32 y_min = rect_a.y < rect_b.y ? rect_a.y : rect_b.y;
		Float_32 y_max = rect_a.y + rect_a.h > rect_b.y + rect_b.h ? rect_a.y + rect_a.h : rect_b.y + rect_b.h;

		if ((x_max - x_min < rect_a.w + rect_b.w) && (y_max - y_min < rect_a.h + rect_b.h))
			return true;

		return false;
	}

	Bool_8 point_in_shape(Vector2<Float_32>* points, Uint_32 count, Vector2<Float_32> p)
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
		Rect<Float_32> _rect{ x_min, y_min, (x_max - x_min), (y_max - y_min) };
		if (!point_in_AABB(p, _rect))
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

	Float_32 solve_line(Vector2<Float_32> v1, Vector2<Float_32> v2, Vector2<Float_32> p)
	{
		Float_32 A = v1.y - v2.y;
		Float_32 B = v1.x - v2.x;
		Float_32 C = v1.x * v2.y - v2.x*v1.y;

		// solve line 1 for the two points in line 2
		return A * p.x + B * p.y + C;

	}

	Vector2<Float_32> support(Vector2<Float_32> dir, Collider& col_A, Collider& col_B)
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

	Bool_8 do_simplex(std::vector<Vector2<Float_32>>& simplex, Vector2<Float_32>& dir)
	{
		static Vector2<Float_32> origin{ 0,0 };
		if (simplex.size() == 3)	//we got a triangle. so check if it contains origin
		{
			// for side A of triangle
			Vector3<Float_32> vec1 = simplex[2] - simplex[1];
			Vector3<Float_32> vec2 = -simplex[1];
			Vector3<Float_32> perp = (vec1^vec2) ^ vec1;

			Float_32 _dotA = perp * (simplex[0] - simplex[2]);

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

			Float_32 _dotB = perp * (simplex[1] - simplex[2]);

			if (_dotB < 0) // in B region
			{
				dir = perp;
				simplex.erase(simplex.begin() + 1);
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

	Bool_8 intersect_GJK(Collider& col_A, Collider& col_B)
	{
		static Vector2<Float_32> origin{ 0,0 };

		Vector2<Float_32> dir{ 1,0 };
		Vector2<Float_32> p = support(dir, col_A, col_B);
		simplex.clear();
		simplex.push_back(p);

		dir = -p;

		int max_it = 100;
		do {
			p = support(dir, col_A, col_B);

			Float_32 _dot = dir * p;
			if (_dot < 0) // extreme point in direction of origin doesn't reach origin. so can't intersect
				return false;
			simplex.push_back(p);
			if (do_simplex(simplex, dir))
				return true;

		} while (max_it--);

		return false;

	}

	Vector2<Float_32> do_EPA(Collider& col_A, Collider& col_B)
	{
		static Float_32 tolerance = 0.0001;
		Float_32 current_dist = 0;
		Float_32 last_dist = 0;

		int max_it = 10;
		Float_32 delta = 0;
		while (max_it--)
		{
			Vector3<Float_32> edge_n;
			Float_32 min_dist = std::numeric_limits<Float_32>::infinity();
			Uint_32 e_i = 0;

			//find normal to closest edge
			for (int i = 0; i < simplex.size(); i++)
			{
				Vector3<Float_32> p1 = simplex[i];
				Vector3<Float_32> p2 = simplex[(i + 1) % simplex.size()];

				Vector3<Float_32> v1 = p1 - p2;
				Vector3<Float_32> v2 = -p1;
				Vector3<Float_32> _edge_n = (v1^v2) ^ v1;
				_edge_n = -_edge_n;
				_edge_n.normalize();
				Float_32 _dist = _edge_n * p1;
				
				if (_dist < min_dist)
				{
					edge_n = _edge_n;
					min_dist = _dist;
					e_i = i;
				}
			}

			//edge_n.normalize();
			clock_t begin = clock();
			Vector3<Float_32> p = support(edge_n, col_A, col_B);
			clock_t end = clock();
			Double_64 elapsed = (Double_64)(end - begin) / CLOCKS_PER_SEC;

			//last_dist = current_dist;
			current_dist = edge_n * p;
			delta = current_dist - min_dist;
			if ( delta < tolerance)
			{
				return edge_n*current_dist;
			}
			else
			{
				simplex.insert(simplex.begin() + e_i+1, p);
			}
		} 
		
		return Vector2<Float_32>{0, 0};
		
	}

	Rect<Float_32> points_to_rect(Vector2<Float_32>* points)
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

	Rect<Float_32> points_to_rect_with_transform(Vector2<Float_32>*  points, Matrix4x4<Float_32>& transformation)
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

	//std::vector<Vector2<Float_32>> simplex;
}