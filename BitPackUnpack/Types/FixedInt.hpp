/*
 * FixedInt.hpp
 *
 *  Created on: Aug 11, 2015
 *      Author: jcassidy
 */
#ifndef BITPACKUNPACK_TYPES_FIXEDINT_HPP_
#define BITPACKUNPACK_TYPES_FIXEDINT_HPP_

#include "../../FixedInt.hpp"

template<typename BitPackerType,typename T,std::size_t N>BitPackerType& operator&(BitPackerType& P,const FixedInt<T,N>& i)
{
	P.append(N,i.value());
	return P;
}

template<typename BitUnpackerType,typename T,std::size_t N>BitUnpackerType& operator&(BitUnpackerType& U,FixedInt<T,N>& i)
{
	i = FixedInt<T,N>(U.extract(N));
	return U;
}

#endif /* BITPACKUNPACK_TYPES_FIXEDINT_HPP_ */
