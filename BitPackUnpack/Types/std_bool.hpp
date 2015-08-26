/*
 * std_bool.hpp
 *
 *  Created on: Aug 26, 2015
 *      Author: jcassidy
 */

#ifndef BITPACKUNPACK_TYPES_STD_BOOL_HPP_
#define BITPACKUNPACK_TYPES_STD_BOOL_HPP_

template<typename BDPIType>typename std::enable_if< (BDPIType::is_unpacker), BDPIType&>::type operator&(BDPIType& PU,bool& b)
{
	b = PU.extract(1);
	return PU;
}

template<typename BDPIType>typename std::enable_if< (BDPIType::is_packer), BDPIType&>::type operator&(BDPIType& PU,const bool b)
{
	PU.append(1,b);
	return PU;
}

#endif /* BITPACKUNPACK_TYPES_STD_BOOL_HPP_ */
