/*
 * ArrayBitUnpacker.hpp
 *
 *  Created on: Aug 11, 2015
 *      Author: jcassidy
 */

#ifndef BITPACKUNPACK_BDPIAUTOARRAYBITPACKER_HPP_
#define BITPACKUNPACK_BDPIAUTOARRAYBITPACKER_HPP_

#include "BitPacker.hpp"
#include <cassert>
#include <algorithm>
#include <boost/multiprecision/gmp.hpp>


/** BitPacker using a Boost wrapped GMP multiprecision int.
 *
 * This is a convenience for use with Bluespec BDPI, where the function is passed a pre-allocated uint32_t* to store its return
 * value. On destruction, this copies its value to the provided storage space (after checking that the size is correct).
 *
 * NOTE: Since x86 is little-endian, the bytes end up contiguous in memory, regardless of limb size (mp_limb_t). The storage will
 * always have the most significant byte at the highest address.
 *
 */

class BDPIAutoArrayBitPacker : public BitPacker<boost::multiprecision::number<boost::multiprecision::gmp_int>>
{
public:
	using BitPacker<boost::multiprecision::number<boost::multiprecision::gmp_int>>::is_packer;

	BDPIAutoArrayBitPacker(unsigned bits,uint32_t* p) :
		BitPacker<boost::multiprecision::number<boost::multiprecision::gmp_int>>(bits,zeroBits_(bits)),			// pre-allocate the necessary bits
		p_((mp_limb_t*)p) {}

	~BDPIAutoArrayBitPacker()
	{
		if (p_)
		{
			const mpz_t *t = &BitPacker<boost::multiprecision::number<boost::multiprecision::gmp_int>>::value().backend().data();
			assert(t);
			assert(bits());
			int Nl = (bits()+(sizeof(mp_limb_t)<<3)-1)/(sizeof(mp_limb_t)<<3);
			assert(t[0]->_mp_alloc == Nl);
			assert(t[0]->_mp_d);
			std::reverse_copy((mp_limb_t*)t[0]->_mp_d,(mp_limb_t*)t[0]->_mp_d+Nl,p_);
		}
	}

private:
	// return a zero gmp_int with the specified number of bits allocated
	static boost::multiprecision::gmp_int zeroBits_(unsigned bits)
	{
		mpz_t i;
		mpz_init2(i,bits);
		return i;
	}
	mp_limb_t* p_=nullptr;
};



#endif /* BITPACKUNPACK_BDPIAUTOARRAYBITPACKER_HPP_ */
