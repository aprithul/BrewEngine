#include "Scripts/FooScript.hpp"
#include <sstream>
#include "EntityManagementSystemModule.hpp"
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
}

void Foo::end()
{
}


Foo::~Foo()
{
	LOG(LOGTYPE_GENERAL, "Foo destructor");
}