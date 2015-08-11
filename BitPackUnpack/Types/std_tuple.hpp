/*
 * std_tuple.hpp
 *
 *  Created on: Aug 11, 2015
 *      Author: jcassidy
 */

#ifndef BITPACKUNPACK_STD_TUPLE_HPP_
#define BITPACKUNPACK_STD_TUPLE_HPP_

#include <tuple>
#include <type_traits>

namespace detail {

// terminal case (N==size) - do nothing
template<typename BDPIType,std::size_t N,typename... TupleElements> void handleTupleElements(
		BDPIType& PU,
		std::tuple<TupleElements...>& t,
		typename std::enable_if< (N == std::tuple_size<std::tuple<TupleElements...>>::value), int>::type =0)
{}

// recursive case (process a tuple and call for next)
template<typename BDPIType,std::size_t N,typename... TupleElements> void handleTupleElements(
		BDPIType& PU,
		std::tuple<TupleElements...>& t,
		typename std::enable_if< (N < std::tuple_size<std::tuple<TupleElements...>>::value), int>::type =0)
{
	PU & std::get<N>(t);
	handleTupleElements<BDPIType,N+1,TupleElements...>(PU,t);
}


// terminal case (N==size) - do nothing
template<typename BDPIType,std::size_t N,typename... TupleElements> void handleTupleElements(
		BDPIType& PU,
		const std::tuple<TupleElements...>& t,
		typename std::enable_if< (N == std::tuple_size<std::tuple<TupleElements...>>::value), int>::type =0)
{}


template<typename BDPIType,std::size_t N,typename... TupleElements> void handleTupleElements(
		BDPIType& PU,
		const std::tuple<TupleElements...>& t,
		typename std::enable_if< (N < std::tuple_size<std::tuple<TupleElements...>>::value), int>::type =0)
{
	PU & std::get<N>(t);
	handleTupleElements<BDPIType,N+1,TupleElements...>(PU,t);
}

};

template<typename BDPIType,typename... TupleElements>BDPIType& operator&(BDPIType& PU,std::tuple<TupleElements...>& t)
{
	detail::handleTupleElements<BDPIType,0,TupleElements...>(PU,t);
	return PU;
}

template<typename BDPIType,typename... TupleElements>BDPIType& operator&(BDPIType& PU,const std::tuple<TupleElements...>& t)
{
	detail::handleTupleElements<BDPIType,0,TupleElements...>(PU,t);
	return PU;
}



#endif /* BITPACKUNPACK_STD_TUPLE_HPP_ */
