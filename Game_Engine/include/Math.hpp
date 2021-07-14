#ifndef MATH_HPP
#define MATH_HPP

#include "Types.hpp"
#include <limits>

#define EPSILON 0.00001
#define PI 3.14159265

namespace PrEngine
{

	template<typename T>
	T clamp(T value, T low, T high)
	{
		if (high < low)
			return value;
		if (value > high)
			return high;
		if (value < low)
			return low;
		else
			return value;
	}

	template<typename T>
	Int_32 sign(T v)
	{
		if (v >= 0)
			return 1;
		else return -1;
	}

	template<typename T>
	T abs(T v)
	{
		if (v > 0)
			return v;
		else if (v < 0)
			return -v;
		else
			return 0;
	}
}


#endif