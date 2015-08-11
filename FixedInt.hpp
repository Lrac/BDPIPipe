/*
 * FixedInt.hpp
 *
 *  Created on: Jul 23, 2015
 *      Author: jcassidy
 */

#ifndef FIXEDINT_HPP_
#define FIXEDINT_HPP_

#include <cstddef>
#include <iostream>
#include <iomanip>

#include <limits>

/** A fixed-bit-width (signed or unsigned) integer.
 *
 * @tparam T	Container type (must have appropriate number of bits, and its signed/unsigned status determines the signedness)
 * @tparam N	Number of bits
 */

template<typename T,std::size_t N>class FixedInt;

template<typename T,std::size_t N>class FixedInt
{
public:
	FixedInt(T i=T()) : value_(i){}

	operator T() const { return value_; }

	unsigned hexdigits() const { return (N+3)>>2; }
	unsigned bits() const { return N; }

	T value() const { return value_; }

private:
	T value_;
};


template<typename T,std::size_t N>std::ostream& operator<<(std::ostream& os,const FixedInt<T,N> i)
{
	return os << std::hex << std::setw(i.hexdigits()) << std::setfill('0') << std::uppercase << T(i);
}

template<typename T,std::size_t N>std::istream& operator>>(std::istream& is,FixedInt<T,N>& i)
{
	T t;
	is >> t;
	i = T(t);
	return is;
}

namespace std {

template<typename T,std::size_t N>class numeric_limits<FixedInt<T,N>>
{
public:
	static constexpr bool is_specialized=true;
	static constexpr T min(){ return std::numeric_limits<T>::is_signed ? (T(1) << T(N-1)) : 0; }
	static constexpr T max(){ return min()-1; }
	static constexpr T lowest(){ return min(); }
	static constexpr int digits = std::numeric_limits<T>::is_signed ? N-1 : N;			// # non-sign bits
	static constexpr int digits10 = lrint(ceil(log10(max())));
	static constexpr int max_digits10 = digits10;

	static constexpr bool is_signed = numeric_limits<T>::is_signed;
	static constexpr bool is_integer = numeric_limits<T>::is_integer;
	static constexpr bool is_exact = true;
	static constexpr int radix = 2;

	// float-like stuff that doesn't really make sense
	static constexpr T epsilon=1;
	static constexpr T round_error=1;

	// int min_exponent
	// int min_exponent10
	// int max_exponent
	// int max_exponent10

	static constexpr bool has_infinity=false;

	static constexpr bool has_quiet_NaN=false;
	static constexpr bool has_signaling_NaN=false;

	// has_denorm
	// has_denorm_loss
	// infinity()
	// quiet_NaN()
	// signaling_NaN()
	// denorm_min()

	static constexpr bool is_iec559=false;
	static constexpr bool is_bounded=true;
	static constexpr bool is_modulo=true;
	static constexpr bool traps=false;
	static constexpr bool tinyness_before=false;

	//static constexpr round_style = round_indeterminate;
};
};



#endif /* FIXEDINT_HPP_ */
