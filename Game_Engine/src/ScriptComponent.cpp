#include "ScriptComponent.hpp"

namespace PrEngine{

	Script::Script() : Component(COMP_SCRIPT)
	{

	}

	Script::~Script()
	{

	}


	std::string Script::to_string()
	{
		return "<Runtime Script>";
	}
}