#include <BDPIPipe/BDPIStreamFunction.hpp>
#include <BDPIPipe/BitPackUnpack/Types/FixedInt.hpp>
#include <BDPIPipe/BitPackUnpack/Types/std_tuple.hpp>
#include <BDPIPipe/FixedInt.hpp>
#include <functional>

/** A very simple example using the wrapper around a lambda that adds two values.
 *
 */

extern "C" {
	uint64_t bsv_makeAdd_18b_4b()
	{
		return reinterpret_cast<uint64_t>(new BDPIStreamFunction<
			std::function<unsigned(unsigned,unsigned)>,
			FixedInt<unsigned,19>,
			FixedInt<unsigned,18>,
			FixedInt<unsigned,4>>
			([](unsigned a,unsigned b){ return a+b; })
		);
	}
}
