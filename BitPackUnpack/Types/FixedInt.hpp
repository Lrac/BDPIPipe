/*
 * FixedInt.hpp
 *
 *  Created on: Aug 11, 2015
 *      Author: jcassidy
 */
#ifndef BITPACKUNPACK_TYPES_FIXEDINT_HPP_
#define BITPACKUNPACK_TYPES_FIXEDINT_HPP_

#include "../../FixedInt.hpp"

template<typename BDPIType,typename T,std::size_t N>typename std::enable_if< (BDPIType::is_unpacker), BDPIType&>::type
		operator&(BDPIType& U,FixedInt<T,N>& i)
{
	i = FixedInt<T,N>(U.template extract<T>(N));
	return U;
}

template<typename BDPIType,typename T,std::size_t N>typename std::enable_if< (BDPIType::is_packer), BDPIType&>::type
	operator&(BDPIType& P,const FixedInt<T,N>& i)
{
	P.append(N,i.value());
	return P;
}


#endif /* BITPACKUNPACK_TYPES_FIXEDINT_HPP_ */
