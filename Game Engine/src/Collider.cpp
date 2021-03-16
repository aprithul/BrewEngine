#include "Collider.hpp"
#include "PhysicsModule.hpp"
#include "EntityManagementSystemModule.hpp"

namespace PrEngine
{
	Collider::Collider() :Component(COMP_COLLIDER)
	{
		graphic_id = 0;
		transform_id= 0;
	}


	std::string Collider::to_string()
	{
		std::string s = "";
		return s;
	}
}