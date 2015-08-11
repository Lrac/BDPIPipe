/*
 * Streams.cpp
 *
 *  Created on: Jan 11, 2015
 *      Author: jcassidy
 */

#include <cstdint>

using namespace std;

#include "BDPIStreamBase.hpp"

extern "C" {

//// Stream accessors

	uint8_t bsv_checkEndOfStream(uint64_t sptr)
	{
		return reinterpret_cast<BDPIStreamBase*>(sptr)->endOfStream();
	}

	void bsv_writeStreamP(uint64_t sptr,const uint32_t Nb,const uint32_t* arg)
	{
		(*reinterpret_cast<BDPIStreamBase*>(sptr))(0,nullptr,Nb,arg);
	}

	void bsv_readStreamP(uint32_t* ret,uint64_t sptr,const uint32_t Nbret)
	{
		(*reinterpret_cast<BDPIStreamBase*>(sptr))(Nbret,ret,0,nullptr);
	}

	void bsv_readWriteStreamP(uint32_t* ret,uint64_t sptr,const uint32_t Nbret,const uint32_t Nbarg,const uint32_t* arg)
	{
		(*reinterpret_cast<BDPIStreamBase*>(sptr))(Nbret,ret,Nbarg,arg);
	}

}
