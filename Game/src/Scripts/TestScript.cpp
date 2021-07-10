#include "Scripts/TestScript.hpp"
#include "Utils.hpp"
#include "EntityManagementSystemModule.hpp"

using namespace PrEngine;

void Test::start()
{
}

void Test::update()
{
	if (input_manager->keyboard.get_key_down(SDLK_r))
	{
		Uint_32 sc = scripting_system.get_component_id(entity);
		Uint_32 ref = scripting_system.get_component(sc).get_script_ref("Foo");
		if (ref)
			LOG(LOGTYPE_GENERAL, "found script");
		else
			LOG(LOGTYPE_GENERAL, "not found");
	}
}

void Test::end()
{
}

Test::~Test()
{
	//LOG(LOGTYPE_GENERAL, "Test destructed");

}
