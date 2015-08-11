/*
 * std_pair.hpp
 *
 *  Created on: Aug 11, 2015
 *      Author: jcassidy
 */

#ifndef BITPACKUNPACK_TYPES_STD_PAIR_HPP_
#define BITPACKUNPACK_TYPES_STD_PAIR_HPP_

// handle pairs (special case of tuple)
template<typename BDPIType,typename T,typename U>BDPIType& operator&(BDPIType& PU,std::pair<T,U>& p)		{ return PU & p.first & p.second; }
template<typename BDPIType,typename T,typename U>BDPIType& operator&(BDPIType& PU,const std::pair<T,U>&  p)	{ return PU & p.first & p.second; }

#endif /* BITPACKUNPACK_TYPES_STD_PAIR_HPP_ */
