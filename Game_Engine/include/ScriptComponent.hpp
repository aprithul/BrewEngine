#ifndef SCRIPT_COMPONENT_HPP
#define SCRIPT_COMPONENT_HPP

#include "Component.hpp"
namespace PrEngine
{
	struct Script : public Component
	{	
		Script();
		~Script();

		std::string to_string() override;
	};
}
#endif // !SCRIPT_COMPONENT_HPP
