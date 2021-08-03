#ifndef COLLIDER_HPP
#define COLLIDER_HPP
#include <assert.h>
#include "Component.hpp"
#include "Vec.hpp"
#include "Math.hpp"
#include "Utils.hpp"
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

	};


	const Uint_32 Collider_Max_Point_Count = 16;

	struct Collider :public Component
	{
		Vec2f points[Collider_Max_Point_Count];
		Uint_32 graphic_id;
		Uint_32 transform_id;
		Shape2DTypes type;
		Uint_32 point_count;

		Collider();
		Collider(const Collider&);
		void initialize() override;
		void start() override;
		void update(Float_32 dt) override;
		void end() override;
		std::string to_string() override;
	};
	
	struct Contact
	{
		Vec2f position;
		Vec2f normal;
		Vec2f rA;
		Vec2f rB;
		Uint_32 Rigidbody2d_a;
		Uint_32 Rigidbody2d_b;
		Float_32 depth;
		Float_32 normal_mass;
		Float_32 tangent_mass;
	};

	struct ContactManidfold
	{
		
		


	};

	struct SimplexPoint
	{
		Vec2f cso_point;
		Vec2f support_point_1;
		Vec2f support_point_2;
	};

	Bool_8 point_in_AABB(Vec2f p, Rect<Float_32>& rect);

	Bool_8 intersect_line_line(Vec2f l1_p1, Vec2f l1_p2, Vec2f l2_p1, Vec2f l2_p2);

	Bool_8 intersect_AABB_AABB(Rect<Float_32>& rect_a, Rect<Float_32>& rect_b);

	Bool_8 point_in_shape(Vec2f* shape_points, Uint_32 count, Vec2f p);

	Float_32 solve_line(Vec2f v1, Vec2f v2, Vec2f p);

	Vec2f support(Vec2f dir, Collider& col_A);

	//Bool_8 do_simplex(std::vector<Vec2f>& simplex, Vec2f& dir);

	Bool_8 intersect_GJK(const Collider& col_A, const Collider& col_B, std::vector<SimplexPoint>& simplex);
	
	Vec2f do_EPA(const Collider& col_A, const Collider& col_B, std::vector<SimplexPoint>& simplex);

	// useful for drawing bounding rects
	Rect<Float_32> points_to_rect(Vec2f* points, const Mat4x4& transformation);
	Rect<Float_32> points_to_rect(Vec3f* points, const Mat4x4& transformation);
	Rect<Float_32> points_to_rect(Vec2f* points);
	Rect<Float_32> points_to_rect(Vec3f* points);
	Rect<Float_32> points_to_rect_with_transform(Vec2f*  points, Mat4x4& transformation);
	
	

}
#endif 
