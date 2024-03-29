#ifndef PHYSICS_MODULE_HPP
#define PHYSICS_MODULE_HPP

#include "Module.hpp"
#include "Vec.hpp"
#include "Collider.hpp"
#include "Rigidbody2D.hpp"
#include "Math.hpp"
#include "Constants.hpp"
#include "Utils.hpp"
#include "ComponentSystem.hpp"
#include <vector>
#include <queue>
namespace PrEngine {
	
	

	struct PhysicsModule : public Module
	{
		static ComponentSystem<Rigidbody2D> rigidbody2d_system;
		static ComponentSystem<Collider> collider_system;
		static std::vector<std::thread> contact_generator_threads;

		static Vec2f gravity;
		static std::vector<Contact> contacts;
		PhysicsModule(std::string name, Int_32 priority);
		~PhysicsModule();

		Uint_32 point_in_any_shape(Vec2f p);
		bool col_point_box(Vec2f p, Vec2f* box);


		void start() override;
		void update() override;
		void end() override;

	private:
		static void generate_contact(int i, int j);
		static void make_thread_pool();
	};

	extern PhysicsModule* physics_module;
}

#endif