/*
 * Serializable.hpp
 *
 *  Created on: Sep 18, 2020
 *      Author: cobre
 */

#ifndef INCLUDE_SERIALIZABLE_HPP_
#define INCLUDE_SERIALIZABLE_HPP_

#include <string>
namespace PrEngine{

	class Serializable
	{
	public:
		virtual ~Serializable()=0;
		virtual std::string to_string() = 0;
	};
}


#endif /* INCLUDE_SERIALIZABLE_HPP_ */
