/*
 * FixedPoint.hpp
 *
 *  Created on: Jul 23, 2015
 *      Author: jcassidy
 */

#ifndef FIXEDPOINT_HPP_
#define FIXEDPOINT_HPP_

/** A fixed-point integer class, which may be signed or unsigned.
 * Either I or F may be negative, reflecting (respectively) maximum values less than one, or steps greater than one.
 * Contains a total of I+F bits, including sign.
 *
 * Bit packing is provided through inheritance from FixedInt.
 *
 * TODO: Add std::numeric_limits<> specialization
 *
 * @tparam T	Underlying type (signed/unsigned is specified by this type)
 * @tparam I	Number of Integer (whole-number bits), which may be negative
 * @tparam F	Number of Fractional bits, may also be negative (I+F must be >0 though)
 */

#include "FixedInt.hpp"

template<typename T,int I,int F>struct FixedPoint : public FixedInt<T,I+F> {
public:
	using FixedInt<T,I+F>::value;

	FixedPoint() : FixedInt<T,I+F>(0){}

	FixedPoint(float f)	: FixedInt<T,I+F>(T(f*recip_k)) {}
	FixedPoint(double d): FixedInt<T,I+F>(T(d*recip_k)) {}

	explicit operator float()  const { return k*float(value());  }
	explicit operator double() const { return k*double(value()); }

	bool equals(double x) const { return FixedPoint<T,I,F>(x).value() == value(); }

private:
	static constexpr double k 		= ldexp(1.0,-F);
	static constexpr double recip_k = ldexp(1.0,F);
};





#endif /* FIXEDPOINT_HPP_ */
