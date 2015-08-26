/*
 * InputArray.hpp
 *
 *  Created on: Aug 11, 2015
 *      Author: jcassidy
 */

#ifndef BITPACKUNPACK_CONSTGMPARRAY_HPP_
#define BITPACKUNPACK_CONSTGMPARRAY_HPP_

#include <boost/multiprecision/gmp.hpp>

/** Const non-owning reference to storage allocated by GMP mpz_t.
 *
 * This does some low-level hacking to avoid copying, and provides a nice Boost::multiprecision wrapped value.
 */

class ConstGMPArray
{
public:
	ConstGMPArray(unsigned bits,const uint32_t* p)
	{
		mpz_t *t = &x_.backend().data();

		// calculate number of GMP limbs in the value
		std::size_t Nl = bits==0 ? 0 : (bits+(sizeof(mp_limb_t)<<3)-1)/(sizeof(mp_limb_t)<<3);

		if (p)
		{
			assert(bits);

			// Setup the GMP int to point to the already-existing uint32_t* rather than allocating new
			t[0]->_mp_alloc=t[0]->_mp_size=Nl;
			t[0]->_mp_d=(reinterpret_cast<mp_limb_t*>((uint32_t*)p));			// cast away constness
		}
		else
			mpz_init(*t);
	}

	// release the storage; we do not actually own the memory so should not deallocate it
	// (Boost destructor will do so automatically, causing nastiness)
	~ConstGMPArray()
	{
		x_.backend().data()->_mp_alloc=x_.backend().data()->_mp_size=0;
		x_.backend().data()->_mp_d=nullptr;
	}

	unsigned bits() const { return bits_; }

	const boost::multiprecision::number<boost::multiprecision::gmp_int>& value() const
			{ assert(x_.backend().data()[0]._mp_d); return x_; }

private:
	boost::multiprecision::number<boost::multiprecision::gmp_int> x_;
	unsigned bits_=0;
};

template<class Output,class BitVector>Output bitslice(const BitVector&,unsigned,unsigned);

template<class Output>Output bitslice(const ConstGMPArray& v,unsigned from,unsigned downto)
	{ return bitslice<Output> (v.value(),from,downto); }


#endif /* BITPACKUNPACK_CONSTGMPARRAY_HPP_ */
