#ifndef TEST_SCRIPT_HPP
#define TEST_SCRIPT_HPP

#include "Script.hpp"

struct Test : public PrEngine::Script
{
	// Inherited via Script
	void awake() override;
	void start() override;
	void update() override;
	void end() override;
};

#endif // !FOO

