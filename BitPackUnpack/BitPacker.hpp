/*
 * BitPacker.hpp
 *
 *  Created on: Aug 11, 2015
 *      Author: jcassidy
 */

#ifndef BITPACKUNPACK_BITPACKER_HPP_
#define BITPACKUNPACK_BITPACKER_HPP_


/** A class for packing values into a bit vector. It fills from the left (most-significant) bit downwards.
 * Uses bit-shifting and a counter to maintain the current position.
 *
 * @tparam Integral		An integral type supporting the usual operations (assignment, shifts, bitwise and/or)
 */

template<typename Integral>class BitPacker {
public:
	static constexpr bool is_packer=true;

	explicit BitPacker(unsigned bits,Integral x=0) : bits_(bits),remaining_(bits),x_(x){}

	BitPacker(const BitPacker&) = delete;
	BitPacker(BitPacker&&) = delete;
	BitPacker& operator=(const BitPacker&) = delete;

	~BitPacker()
	{
		if (remaining_ != 0)
			std::cerr << "WARNING: Unused bits in BitPacker (filled " << bits_-remaining_ << '/' << bits_ << ")" << std::endl;
	}

	template<typename T>void append(size_t N,const T& x)
	{
		if (N > remaining_)
			throw std::out_of_range("BitPacker::append");

		Integral mask = 1;
		mask <<= N;
		--mask;

		Integral tmp = x;
		tmp &= mask;

		remaining_-=N;
		tmp <<= remaining_;

		x_ |= tmp;
	}

	unsigned bits() const { return bits_; }
	unsigned remaining() const { return remaining_; }

	const Integral& value() const { return x_; }

private:
	unsigned bits_=0;
	unsigned remaining_=0;
	Integral x_=0;
};



#endif /* BITPACKUNPACK_BITPACKER_HPP_ */
