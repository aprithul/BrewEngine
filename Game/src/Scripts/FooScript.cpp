#include "Scripts/FooScript.hpp"
#include <sstream>
#include "EntityManagementSystemModule.hpp"
#include "PhysicsModule.hpp"
#include "InputModule.hpp"
using namespace PrEngine;

void Foo::start()
{
}

void Foo::update()
{
	/*Uint_32 tr_id = transform_entity_id[entity];
	Point3d p = transforms[tr_id].get_global_position();
	std::stringstream to_string;
	to_string << p.x << "," << p.y << "," << p.z << std::endl;
	LOG(LOGTYPE_GENERAL, to_string.str());*/
	if (input_manager->keyboard.get_key(SDLK_p))
	{
		Uint_32 rb_id = PhysicsModule::rigidbody2d_system.get_component_id(entity);
		Rigidbody2D& rb = PhysicsModule::rigidbody2d_system.get_component(rb_id);
		rb.add_force({ 10, 10 });
	}

	if (input_manager->keyboard.get_key(SDLK_o))
	{
		Uint_32 rb_id = PhysicsModule::rigidbody2d_system.get_component_id(entity);
		Rigidbody2D& rb = PhysicsModule::rigidbody2d_system.get_component(rb_id);
		rb.add_force_at_point({ 0, 10 }, { 1,0 });
	}


}

void Foo::end()
{
}


Foo::~Foo()
{
	LOG(LOGTYPE_GENERAL, "Foo destructor");
}