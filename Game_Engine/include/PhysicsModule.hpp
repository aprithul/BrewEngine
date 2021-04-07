#ifndef PHYSICS_MODULE_HPP
#define PHYSICS_MODULE_HPP

#include "Module.hpp"
#include "Vec.hpp"
#include "Collider.hpp"
#include "Math.hpp"
#include "Utils.hpp"
#include <vector>
#include <queue>
namespace PrEngine {


	struct PhysicsModule : public Module
	{
		static std::vector<Contact> contacts;

		PhysicsModule(std::string name, Int_32 priority);
		~PhysicsModule();

		Uint_32 point_in_any_shape(Vec2f p);
		bool col_point_box(Vec2f p, Vec2f* box);


		void start() override;
		void update() override;
		void end() override;
	};

	extern PhysicsModule* physics_module;
}

#endif