/*
 * BDPIStreamFunction.hpp
 *
 *  Created on: Aug 11, 2015
 *      Author: jcassidy
 */

#ifndef BDPISTREAMFUNCTION_HPP_
#define BDPISTREAMFUNCTION_HPP_

#include "BDPIStreamBase.hpp"

#include "BitPackUnpack/BDPIAutoArrayBitPacker.hpp"
#include "BitPackUnpack/BitUnpacker.hpp"
#include "BitPackUnpack/ConstGMPArray.hpp"

#include "BitPackUnpack/Types/std_tuple.hpp"

/** A wrapper for general functions taking N arguments and returning some type Return.
 *
 */


template<class Fn,typename Return,typename... Args>class BDPIStreamFunction : public BDPIStreamBase
{
public:
	template<class... CtorArgs>BDPIStreamFunction(CtorArgs... ctorargs) : f_(ctorargs...){}
	Fn& func(){ return f_; }

private:
	virtual void operator()(BDPIAutoArrayBitPacker& oa,BitUnpacker<ConstGMPArray>& ia)
	{
		// unpack the arguments
		std::tuple<Args...> iTpl = proto_;
		ia & iTpl;

		// call the function with its natural argument types
		callAndSetOutput<Return>(oa,f_,iTpl);
	}

	Fn f_;
	std::tuple<Args...> proto_;		// prototype input (may carry things like bit widths etc)
};

// Specializations

template<class Fn,typename... Args> 	using BDPIOStreamFunction = BDPIStreamFunction<Fn,void,Args...>;
template<class Fn,typename Return> 		using BDPIIStreamFunction = BDPIStreamFunction<Fn,Return>;


#endif /* BDPISTREAMFUNCTION_HPP_ */
