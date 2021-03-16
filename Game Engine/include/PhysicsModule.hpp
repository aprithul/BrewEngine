#ifndef PHYSICS_MODULE_HPP
#define PHYSICS_MODULE_HPP

#include "Module.hpp"
#include "Vector3.hpp"
#include "Collider.hpp"
#include "Utils.hpp"
#include <vector>
#include <queue>
namespace PrEngine {


	struct PhysicsModule : public Module
	{
		//static std::vector<CollisionShape2D> collision_shapes;

		PhysicsModule(std::string name, Int_32 priority);
		~PhysicsModule();

		Uint_32 point_in_any_shape(Vector2<Float_32> p);
		Bool_8 point_in_AABB(Vector2<Int_32> p, Rect& rect);
		bool col_point_box(Vector2<Float_32> p, Vector2<Float_32>* box);
		inline bool point_in_shape(Vector2<Float_32>* points, Uint_32 count, Vector2<Float_32> p)
		{
			Bool_8 inside = true;
			for (Uint_32 _i = 0; _i < count; _i++)
			{

				Vector2<Float_32> v1 = points[_i] - p;
				Vector2<Float_32> v2 = points[(_i + 1) % count] - p;

				Float_32 ang = Vector2<Float_32>::angle(v1, v2);
				if (ang > 180)
				{
					inside = false;
					break;
				}
			}
			return inside;
		}
		void start() override;
		void update() override;
		void end() override;
	};

	extern PhysicsModule* physics_module;
}

#endif