/*
 * Test_BDPIPipe_Unit.cpp
 *
 *  Created on: Jul 23, 2015
 *      Author: jcassidy
 */

#include "BitPackUnpack/BitUnpacker.hpp"
#include <boost/multiprecision/gmp.hpp>
#include "BitPackUnpack/BDPIAutoArrayBitPacker.hpp"

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Test_BDPIPipe_Unit

#include <boost/test/unit_test.hpp>

#include "BitPackUnpack/Types/std_pair.hpp"
#include "BitPackUnpack/Types/std_tuple.hpp"
#include "BitPackUnpack/Types/std_array.hpp"
#include "BitPackUnpack/Types/FixedInt.hpp"
#include "BitPackUnpack/Types/Pad.hpp"

#include "FixedInt.hpp"

using namespace std;

BOOST_AUTO_TEST_CASE( myTestUInt8 )
{
	unsigned char x(0xf0);

	BOOST_CHECK( bitslice<unsigned char>(x,7U,0U) == 0xf0 );
	BOOST_CHECK( bitslice<unsigned char>(x,7U,4U) == 0x0f );
	BOOST_CHECK( bitslice<unsigned char>(x,3U,0U) == 0x00 );
	BOOST_CHECK( bitslice<unsigned char>(x,0U,0U) == 0x00 );
	BOOST_CHECK( bitslice<unsigned char>(x,7U,7U) == 0x01 );

	BOOST_CHECK_THROW ( bitslice<unsigned char>(x,6U,7U), std::logic_error);
//	BOOST_CHECK_THROW ( bitslice<unsigned char>(x,8U,7U), std::out_of_range);
}

BOOST_AUTO_TEST_CASE( myTestp128 )
{
	boost::multiprecision::mpz_int x;

	x = 0x0ff;
	x <<= 100;

	BOOST_CHECK( bitslice<unsigned>(x, 99U, 68U) == 0);
	//BOOST_CHECK_THROW (bitslice<unsigned>(x,99U, 67U), std::logic_error); // error check currently disabled
	BOOST_CHECK( bitslice<unsigned>(x,107U,100U) == 0xff);
	BOOST_CHECK( bitslice<unsigned>(x,127U,116U) == 0);
	//BOOST_CHECK_THROW ( bitslice<unsigned>(x,128U,128U), std::out_of_range); // error check currently disabled
	BOOST_CHECK_THROW ( bitslice<unsigned>(x,100U,101U), std::logic_error);
}

BOOST_AUTO_TEST_CASE( outp128 )
{
	const unsigned N = 128;
	uint32_t v[N/32];

	BDPIAutoArrayBitPacker oa(N,&v[0]);

	oa.append(16,0xdead);
	oa.append(32,0xbaadc0de);
	oa.append(4,0xa);
	oa.append(4,0x5);
	oa.append(4,0x5);
	oa.append(4,0x0);
	oa.append(63,0x0);
	oa.append(1,0x1);

	boost::multiprecision::mpz_int expect("0xdeadbaadc0dea5500000000000000001");

	BOOST_CHECK(oa.value() == expect);

	uint32_t v2[N/32];

	BDPIAutoArrayBitPacker oa2(N,&v2[0]);

	oa2 & FixedInt<unsigned,16>(0xdead)
			& FixedInt<unsigned,32>(0xbaadc0de)
			& std::make_pair(FixedInt<unsigned char,4>(0xa), FixedInt<unsigned char,8>(0x55))
			& FixedInt<unsigned long long,64>(0x0)
			& std::array<FixedInt<unsigned char,1>,3>{ 0,0,0 }
			& FixedInt<unsigned char,1>(0x1);

	BOOST_CHECK(oa2.value() == expect);
}

#include "BitPackUnpack/ConstGMPArray.hpp"

BOOST_AUTO_TEST_CASE( inp128 )
{
	const unsigned N=128;
	const unsigned Nw=N/32;
	uint32_t v[Nw];

	std::fill(v,v+Nw,0);

	v[Nw-1] = 0xffeeddcc;

	ConstGMPArray GA(N,&v[0]);
	BOOST_CHECK_EQUAL(bitslice<unsigned char>(GA, N-1, N-8), 0xff);
	BOOST_CHECK_EQUAL(bitslice<unsigned char>(GA,  7U, 0U),  0x00);

	BOOST_CHECK_EQUAL((GA.value() >> 120), 			0xff);			// highest address byte is 0xff -> should be MSB
	BOOST_CHECK_EQUAL((GA.value() >>  96) & 0xff,	0xcc);			// 4th highest address byte is LS byte of top word (0xcc)



	BitUnpacker<ConstGMPArray> oa(N,&v[0]);

	FixedInt<unsigned char,8> expect_ff;
	FixedInt<unsigned,16> expect_eedd;
	FixedInt<unsigned,4> expect_c, expect_c2;

	//FixedInt<boost::multiprecision::number<boost::multiprecision::gmp_int>,76> rest;

	std::tuple<FixedInt<unsigned,4>,FixedInt<unsigned,4>> pc;

	oa & expect_ff & expect_eedd & pc & Pad<20>();// & rest;

	//BOOST_CHECK_EQUAL(rest.value(), 0);
	BOOST_CHECK_EQUAL(expect_ff.value(), 0xff);
	BOOST_CHECK_EQUAL(expect_eedd.value(),0xeedd);
	BOOST_CHECK_EQUAL(get<0>(pc).value(),0xc);
	BOOST_CHECK_EQUAL(get<1>(pc).value(),0xc);
}

#include "FixedInt.hpp"
#include "FixedPoint.hpp"

#include "BitPackUnpack/BDPIAutoArrayBitPacker.hpp"
#include "BitPackUnpack/ConstGMPArray.hpp"

#include "BitPackUnpack/Types/std_bool.hpp"
#include "BitPackUnpack/Types/std_array.hpp"
#include "BitPackUnpack/Types/std_pair.hpp"
#include "BitPackUnpack/Types/std_tuple.hpp"
#include "BitPackUnpack/Types/Pad.hpp"

BOOST_AUTO_TEST_CASE( packunpack )
{
	const unsigned N=110;
	mp_limb_t dst[(N+sizeof(mp_limb_t)*8-1)/sizeof(mp_limb_t)];

	BOOST_CHECK_EQUAL(sizeof(mp_limb_t),8);

	{
		BDPIAutoArrayBitPacker P(N,(uint32_t*)dst);

		std::tuple<FixedInt<unsigned,4>,bool,bool,FixedPoint<signed,3,4>> 	t{4,true,false,-2.7};		// 13b
		std::pair<FixedInt<signed,3>,FixedInt<unsigned,3>> 					p{-2,7};					// 6b
		std::array<FixedInt<unsigned,4>,4>									a{0,1,2,15};				// 16b

		FixedInt<unsigned long long,64>										i=0xdeadbeefbaadc0deULL; 	// 64b

		P & t & p & a & Pad<11>() & i;
	}

	ConstGMPArray A(N,(const uint32_t*)dst);

	cout << std::dec;

	BitUnpacker<ConstGMPArray>	U(N,(const uint32_t*)dst);

	std::tuple<FixedInt<unsigned,4>,bool,bool,FixedPoint<signed,3,4>> 		t;			// 13b
	std::pair<FixedInt<signed,3>,FixedInt<unsigned,3>> 						p;			// 6b
	std::array<FixedInt<unsigned,4>,4>										a;			// 16b

	FixedInt<unsigned long long,64>											i; 			// 64b

	U & t & p & a & Pad<11>() & i;

	BOOST_CHECK_EQUAL(get<0>(t).value(),4);
	BOOST_CHECK_EQUAL(get<1>(t),true);
	BOOST_CHECK_EQUAL(get<2>(t),false);
	BOOST_CHECK(get<3>(t).equals(-2.7));

	BOOST_CHECK_EQUAL(p.first.value(),-2);
	BOOST_CHECK_EQUAL(p.second.value(),7);

	BOOST_CHECK_EQUAL(a[0].value(),0);
	BOOST_CHECK_EQUAL(a[1].value(),1);
	BOOST_CHECK_EQUAL(a[2].value(),2);
	BOOST_CHECK_EQUAL(a[3].value(),15);

	BOOST_CHECK_EQUAL(i.value(),0xdeadbeefbaadc0deULL);
}



