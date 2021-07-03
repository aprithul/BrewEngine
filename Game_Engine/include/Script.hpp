#ifndef SCRIPT_HPP
#define SCRIPT_HPP
#include "Types.hpp"
namespace PrEngine
{
	struct Script
	{
		Uint_32 entity;
		virtual void start() = 0;
		virtual void update() = 0;
		virtual void end() = 0;

		virtual ~Script() = 0;

	};

	inline Script::~Script(){}
}

#endif