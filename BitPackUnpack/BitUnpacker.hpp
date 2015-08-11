/*
 * BitUnpacker.hpp
 *
 *  Created on: Aug 11, 2015
 *      Author: jcassidy
 */

#ifndef BITPACKUNPACK_BITUNPACKER_HPP_
#define BITPACKUNPACK_BITUNPACKER_HPP_

#include <cstdint>
#include <iostream>
#include <exception>
#include <stdexcept>

/**
 * @tparam ReadableBitVector
 *
 * ReadableBitVector(v) requires that bitslice<T>(v,from,downto) is well-formed.
 */

/** Extract a bitslice from a bit vector. The underlying type must support shifts, arithmetic, and bitwise operations.
 *
 * TODO: Sign-extension support
 * TODO: Better error checking
 */

template<class Output,class BitVector>Output bitslice(const BitVector& v,unsigned from,unsigned downto)
{
//	 make sure selection range is valid
//	if (from >= bits<BitVector>(v))
//		throw std::out_of_range("bitslice: 'from' value out of range");

	if (downto > from)
		throw std::logic_error("bitslice: direction incorrect");

	std::size_t Nb=from-downto+1;
//	std::size_t No=BDPIBitVectorTraits<T>::maxBits_;

//	if (Nb > No)
//		throw std::logic_error("bitslice: recipient type of insufficient size");

	// NOT TESTED YET
////
////		if (std::numeric_limits<T>::is_signed)
////		{
//			// Need to handle sign-extension properly; shift so left edge of range is at MSB of output var, then shift right
//			// because T is signed, right-shift will do sign extension
//			T o = static_cast<T>(
//				from >= BDPIBitVectorTraits<T>::maxBits_-1 ?
//						((typename BDPIBitStorage<InputType>::Storage)(backend() >> T(from-No))) :
//						((typename BDPIBitStorage<InputType>::Storage)(backend() << T(No-from-1)))
//			);
//
//			return o >> (No-Nb);
//		}
//		else

	BitVector tmp = v;
	tmp >>= downto;

	Output mask = 1;
	mask <<= Nb;
	mask--;

	// formulation below is required when working with GMP because it appears to saturate if tmp > maxvalue(Output)
	tmp &= mask;
	return static_cast<Output>(tmp);
}

template<typename ReadableBitVector>class BitUnpacker {
public:

	static constexpr bool is_unpacker=true;

	template<typename T>BitUnpacker(unsigned bits,T i) : bits_(bits),remaining_(bits),x_(bits,i){}

	BitUnpacker(const BitUnpacker&) = delete;
	BitUnpacker(BitUnpacker&&) = delete;
	BitUnpacker& operator=(const BitUnpacker&) = delete;

	~BitUnpacker()
	{
		if (remaining_)
			std::cerr << "WARNING: Unused bits in BitUnpacker (consumed " << bits_-remaining_ << '/' << bits_ << ")" << std::endl;
	}

	/// Extract bits from the archive using bit slice
	template<typename T=unsigned long long>T extract(std::size_t N)
	{
		if (N > remaining_)
			throw std::out_of_range("BitUnpacker::extract requested more bits than are remaining");

		T o = bitslice<T>(x_,remaining_-1,remaining_-N);
		remaining_ -= N;

		return o;
	}

	void discard(std::size_t N)
	{
		if (N > remaining_)
			throw std::out_of_range("BitUnpacker::discard requested more bits than are remaining");
		remaining_ -=N;
	}

	unsigned bits() const { return bits_; }
	unsigned remaining() const { return remaining_; }

private:
	unsigned bits_=0;
	unsigned remaining_=0;
	const ReadableBitVector x_;
};



#endif /* BITPACKUNPACK_BITUNPACKER_HPP_ */
