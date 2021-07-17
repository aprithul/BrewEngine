#include "Collider.hpp"
#include "PhysicsModule.hpp"
#include "EntityManagementSystemModule.hpp"
#include <time.h>
namespace PrEngine
{
	Collider::Collider() :Component(COMP_COLLIDER)
	{
		initialize();
	}

	void Collider::initialize()
	{
		graphic_id = 0;
		transform_id = 0;
	}

	void Collider::start()
	{

	}

	void Collider::update()
	{

	}

	void Collider::end()
	{

	}

	std::string Collider::to_string()
	{
		std::string _s = std::to_string(COMP_COLLIDER)+","+ std::to_string(collision_shape.type);
		for (int _i = 0; _i < collision_shape.point_count; _i++)
		{
			_s += "," + std::to_string(collision_shape.points[_i].x) + "," + std::to_string(collision_shape.points[_i].y);
		}
		return _s;
	}


	Bool_8 point_in_AABB(Vec2f p, Rect<Float_32>& rect)
	{
		if (p.x > rect.x && p.x < rect.x + rect.w
			&& p.y > rect.y && p.y < rect.y + rect.h)
			return true;
		else
			return false;
	}

	std::vector<Vec2f> simplex;

	Bool_8 intersect_line_line(Vec2f l1_p1, Vec2f l1_p2, Vec2f l2_p1, Vec2f l2_p2)
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
		Float_32 determinant = l1_a * l2_b - l1_b * l2_a;
		if (determinant < EPSILON && determinant > -EPSILON)
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

	Bool_8 point_in_shape(Vec2f* shape_points, Uint_32 count, Vec2f p)
	{
		// at least 3 points needed to contain another point
		if (count < 3)
			return false;

		Float_32 x_min = shape_points[0].x;
		Float_32 x_max = shape_points[0].x;
		Float_32 y_min = shape_points[0].y;
		Float_32 y_max = shape_points[0].y;

		for (int _i = 1; _i < count; _i++)
		{
			if (shape_points[_i].x < x_min)
				x_min = shape_points[_i].x;
			if (shape_points[_i].x > x_max)
				x_max = shape_points[_i].x;
			if (shape_points[_i].y < y_min)
				y_min = shape_points[_i].y;
			if (shape_points[_i].y > y_max)
				y_max = shape_points[_i].y;
		}

		// Can't be inside shape if not in AABB
		Rect<Float_32> _rect{ x_min, y_min, (x_max - x_min), (y_max - y_min) };
		if (!point_in_AABB(p, _rect))
			return false;

		// farther test ( crossing number algorithm )

		// two points of line from outiside of bounding box to point
		Vec2f l1_p1{ x_min - 0.1f, y_min };
		Vec2f l1_p2 = p;

		Int_32 intersection_count = 0;
		// check intersections with every edge of shape
		for (int _i = 0; _i < count; _i++)
		{
			// two points of line 2 ( an edge of shape)
			Vec2f l2_p1 = shape_points[_i];
			Vec2f l2_p2 = shape_points[(_i + 1) % count];

			Bool_8 intersection_res = intersect_line_line(l1_p1, l1_p2, l2_p1, l2_p2);
			if (intersection_res)
				intersection_count++;
		}

		// if odd, then inside
		if ((intersection_count & 1))	// odd number will have 1 as last digit
			return true;

		return false;

	}

	Float_32 solve_line(Vec2f v1, Vec2f v2, Vec2f p)
	{
		Float_32 A = v1.y - v2.y;
		Float_32 B = v1.x - v2.x;
		Float_32 C = v1.x * v2.y - v2.x*v1.y;

		// solve line 1 for the two points in line 2
		return A * p.x + B * p.y + C;

	}

	Vec2f support(Vec2f dir, const Collider& col_A, const Collider& col_B)
	{
		//assert(col_A.transform_id && col_B.transform_id);

		// find max point in A along dir
		Vec2f A_max;
		Float_32 max_proj = -std::numeric_limits<Float_32>::infinity();
		for (Uint_32 i = 0; i < col_A.collision_shape.point_count; i++)
		{
			Vec2f point = (transform_system.get_component(col_A.transform_id).transformation * col_A.collision_shape.points[i]);
			Float_32 proj = Dot(dir, point);
			if (proj > max_proj)
			{
				max_proj = proj;
				A_max = point;
			}
		}

		// find max point in B along  (-dir)
		Vec2f B_max;
		max_proj = -std::numeric_limits<Float_32>::infinity();
		for (Uint_32 i = 0; i < col_B.collision_shape.point_count; i++)
		{
			Vec2f point = (transform_system.get_component(col_B.transform_id).transformation * col_B.collision_shape.points[i]);
			Float_32 proj = Dot(-dir, point);
			if (proj > max_proj)
			{
				max_proj = proj;
				B_max = point;
			}
		}


		return A_max - B_max;
	}

	Bool_8 do_simplex(std::vector<Vec2f>& simplex, Vec2f& dir)
	{
		static Vec2f origin{ 0,0 };
		if (simplex.size() == 3)	//we got a triangle. so check if it contains origin
		{
			// for side A of triangle
			Vec3f vec1 = (Vec3f)(simplex[2] - simplex[1]);
			Vec3f vec2 = (Vec3f)(-simplex[1]);
			Vec3f perp = Cross(Cross(vec1, vec2), vec1);

			Float_32 _dotA = Dot(perp, (Vec3f)(simplex[0] - simplex[2]));

			if (_dotA < 0) // in A region
			{
				dir = (Vec2f)perp;
				simplex.erase(simplex.begin());
				return false;
			}

			//------------------------------------------------

			// for side B of triangle
			vec1 = (Vec3f)(simplex[2] - simplex[0]);
			vec2 = (Vec3f)(-simplex[0]);
			perp = Cross(Cross(vec1, vec2), vec1);

			Float_32 _dotB = Dot(perp , (Vec3f)(simplex[1] - simplex[2]));

			if (_dotB < 0) // in B region
			{
				dir = (Vec2f)perp;
				simplex.erase(simplex.begin() + 1);
				return false;
			}

			///
			//else inside triangle
			return true;



		}
		else if (simplex.size() == 2)
		{
			Vec3f vec1 = (Vec3f)(-simplex[1]);
			Vec3f vec2 = (Vec3f)(simplex[0] - simplex[1]);
			//Float_32 _dot = vec1 * vec2;
			//if (_dot > 0)	// new direction is normal to vec2 in the direction of origin
			//{
			Vec3f perp = Cross(Cross(vec2, vec1), vec2);
			dir = (Vec2f)(perp.GetNormalized());
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

	Bool_8 intersect_GJK(const Collider& col_A, const Collider& col_B)
	{
		static Vec2f origin{ 0,0 };

		Vec2f dir{ 1,0 };
		Vec2f p = support(dir, col_A, col_B);
		simplex.clear();
		simplex.push_back(p);

		dir = -p;

		int max_it = 100;
		do {
			p = support(dir, col_A, col_B);

			Float_32 _dot = Dot(dir, p);
			if (_dot < 0) // extreme point in direction of origin doesn't reach origin. so can't intersect
				return false;
			simplex.push_back(p);
			if (do_simplex(simplex, dir))
				return true;

		} while (max_it--);

		return false;

	}

	Vec2f do_EPA(const Collider& col_A, const Collider& col_B)
	{
		static Float_32 tolerance = 0.0001;
		Float_32 current_dist = 0;
		Float_32 last_dist = 0;

		int max_it = 10;
		Float_32 delta = 0;
		while (max_it--)
		{
			Vec3f edge_n;
			Float_32 min_dist = std::numeric_limits<Float_32>::infinity();
			Uint_32 e_i = 0;

			//find normal to closest edge
			for (int i = 0; i < simplex.size(); i++)
			{
				Vec3f p1 = (Vec3f)simplex[i];
				Vec3f p2 = (Vec3f)(simplex[(i + 1) % simplex.size()]);

				Vec3f v1 = p1 - p2;
				Vec3f v2 = -p1;
				Vec3f _edge_n = Cross(Cross(v1, v2), v1);
				_edge_n = -_edge_n;
				_edge_n.Normalize();
				Float_32 _dist = Dot(_edge_n, p1);
				
				if (_dist < min_dist)
				{
					edge_n = _edge_n;
					min_dist = _dist;
					e_i = i;
				}
			}

			//edge_n.normalize();
			clock_t begin = clock();
			Vec3f p = (Vec3f)support((Vec2f)edge_n, col_A, col_B);
			clock_t end = clock();
			Double_64 elapsed = (Double_64)(end - begin) / CLOCKS_PER_SEC;

			//last_dist = current_dist;
			current_dist = Dot(edge_n, p);
			delta = current_dist - min_dist;
			if ( delta < tolerance)
			{
				return (Vec2f)(edge_n*current_dist);
			}
			else
			{
				simplex.insert(simplex.begin() + e_i+1, (Vec2f)p);
			}
		} 
		
		return Vec2f{0, 0};
		
	}
	Rect<Float_32> points_to_rect(Vec3f* points, const Mat4x4& transformation)
	{
		Rect<Float_32> rect = points_to_rect(points);
		Point3d p = { rect.x, rect.y, 0 };
		p = transformation * p;
		rect.x = p.x;
		rect.y = p.y;
		return rect;
	}

	Rect<Float_32> points_to_rect(Vec2f* points, const Mat4x4& transformation)
	{
		Rect<Float_32> rect = points_to_rect(points);
		Point3d p = { rect.x, rect.y, 0};
		p = transformation * p;
		rect.x = p.x;
		rect.y = p.y;
		return rect;
	}


	Rect<Float_32> points_to_rect(Vec2f* points)
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

	Rect<Float_32> points_to_rect(Vec3f* points)
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

	Rect<Float_32> points_to_rect_with_transform(Vec2f*  points, Mat4x4& transformation)
	{
		Float_32 x_min = std::numeric_limits<Float_32>::infinity();
		Float_32 x_max = -std::numeric_limits<Float_32>::infinity();
		Float_32 y_min = std::numeric_limits<Float_32>::infinity();
		Float_32 y_max = -std::numeric_limits<Float_32>::infinity();

		for (int _i = 1; _i < 4; _i++)
		{
			Vec2f p = transformation * points[_i];
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

	//std::vector<Vec2f> simplex;
}