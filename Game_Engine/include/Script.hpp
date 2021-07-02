#ifndef SCRIPT_HPP
#define SCRIPT_HPP

namespace PrEngine
{
	struct Script
	{
		virtual void start() = 0;
		virtual void update() = 0;
		virtual void end() = 0;

	};
}

#endif