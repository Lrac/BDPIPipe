/*
 * Pad.hpp
 *
 *  Created on: Aug 11, 2015
 *      Author: jcassidy
 */

#ifndef PAD_HPP_
#define PAD_HPP_

template<std::size_t N>class Pad
{
};

template<typename BitPackerType,std::size_t N>
	typename std::enable_if<BitPackerType::is_packer,BitPackerType>::type& operator&(BitPackerType& P,const Pad<N>& i)
{
	P.append(N,0);
	return P;
}

template<typename BitUnpackerType,std::size_t N>
	typename std::enable_if<BitUnpackerType::is_unpacker,BitUnpackerType>::type& operator&(BitUnpackerType& U,Pad<N> i)
{
	U.discard(N);
	return U;
}



#endif /* PAD_HPP_ */
