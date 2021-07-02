#ifndef FOO_SCRIPT_HPP
#define FOO_SCRIPT_HPP

#include "Script.hpp"

struct Foo : public PrEngine::Script
{
	// Inherited via Script
	void start() override;
	void update() override;
	void end() override;
};

#endif // !FOO

