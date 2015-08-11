/*
 * Streams.hpp
 *
 *  Created on: Jan 11, 2015
 *      Author: jcassidy
 */

#ifndef BDPISTREAMBASE_HPP_
#define BDPISTREAMBASE_HPP_

#include <BDPIPipe/BitPackUnpack/BDPIAutoArrayBitPacker.hpp>
#include <BDPIPipe/BitPackUnpack/ConstGMPArray.hpp>
#include <BDPIPipe/BitPackUnpack/BitUnpacker.hpp>

#include <BDPIPipe/BitPackUnpack/Types/FixedInt.hpp>

#include <iostream>
#include <unordered_map>
#include <string>

// TODO: Must be a nicer way to do this?
template<class Return,class Callable>Return call(Callable& c){ return c(); }
template<class Return,class Callable>Return call(Callable& c,const std::tuple<>&){ return c(); }
template<class Return,class Callable,typename T0>Return call(Callable& c,const std::tuple<T0>& t){ return c(std::get<0>(t).value()); }
template<class Return,class Callable,typename T0,typename T1>Return call(Callable& c,const std::tuple<T0,T1>& t){ return c(std::get<0>(t).value(),std::get<1>(t).value()); }
template<class Return,class Callable,typename T0,typename T1,typename T2>Return call(Callable& c,const std::tuple<T0,T1,T2>& t){ return c(std::get<0>(t).value(),std::get<1>(t).value(),std::get<2>(t).value()); }
template<class Return,class Callable,typename T0,typename T1,typename T2,typename T3>Return call(Callable& c,const std::tuple<T0,T1,T2,T3>& t){ return c(std::get<0>(t).value(),std::get<1>(t).value(),std::get<2>(t).value(),std::get<3>(t).value()); }


/** The base object for polymorphic stream objects. For generality, it takes a single variable-sized argument and yields a
 * variable-sized return. The performance overhead to do so is probably not that bad.
 *
 */

class BDPIStreamBase {
public:
	/// The basic function call
	// For istreams, the return value (Nbret, ret) will be 0/null
	// For ostreams, the arguments (Nbarg, arg) will be 0/null
	void operator()(std::size_t Nbret,uint32_t* ret,std::size_t Nbarg,const uint32_t* arg)
	{
		BitUnpacker<ConstGMPArray> iunpack(Nbarg,arg);
		BDPIAutoArrayBitPacker opack(Nbret,ret);

		operator()(opack,iunpack);
	}

	virtual bool endOfStream() const { return false; }
private:

	virtual void operator()(BDPIAutoArrayBitPacker& opack,BitUnpacker<ConstGMPArray>& iunpack)=0;

};


// Pair of functions using SFINAE to ignore the output iff the function returns void
template<typename Return,class Archive,class Fn,typename...Args>
	void callAndSetOutput(Archive& oa,Fn& f,const std::tuple<Args...>& args,
			typename std::enable_if< std::is_same<void,Return>::value, int>::type =0)
{
	call<void>(f,args);

}
template<class Return,class Archive,class Fn,typename...Args>void callAndSetOutput(Archive& oa,Fn& f,const std::tuple<Args...>& args,
		typename std::enable_if< !std::is_same<void,Return>::value, int>::type =0)
{
	oa & call<Return>(f,args);
}



#endif /* BDPISTREAMBASE_HPP_ */
