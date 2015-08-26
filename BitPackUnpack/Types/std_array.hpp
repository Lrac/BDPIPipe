/*
 * std_array.hpp
 *
 *  Created on: Aug 11, 2015
 *      Author: jcassidy
 */

#ifndef BITPACKUNPACK_STD_ARRAY_HPP_
#define BITPACKUNPACK_STD_ARRAY_HPP_

#include <array>

template<typename BDPIType,typename T,std::size_t N>BDPIType& operator&(BDPIType& PU,std::array<T,N>& a)
{
	for(int i=N-1; i>=0; --i)
		PU & a[i];
	return PU;
}

template<typename BDPIType,typename T,std::size_t N>BDPIType& operator&(BDPIType& PU,const std::array<T,N>& a)
{
	for(int i=N-1; i>=0; --i)
		PU & a[i];
	return PU;
}

#endif /* BITPACKUNPACK_STD_ARRAY_HPP_ */
