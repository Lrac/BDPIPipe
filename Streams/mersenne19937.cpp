#include <BDPIPipe/BDPIStreamFunction.hpp>
#include <boost/random/mersenne_twister.hpp>

/** This file creates a function bsv_makeMersenneTwister19937 which implements the named RNG from the Boost library.
 */

#include "../FixedInt.hpp"

extern "C" {
	uint64_t bsv_makeMersenneTwister19937()
	{
		return reinterpret_cast<uint64_t>(new BDPIIStreamFunction<
			boost::random::mt19937,
			FixedInt<uint32_t,32>>
			()
		);
	}
}
