package Test_BDPIPipe;

import StmtFSM::*;
import BDPIPipe::*;
import PAClib::*;
import GetPut::*;

import Assert::*;

import BDPIMersenneTwister::*;
import BDPIExample::*;

import BDPIPipe::*;

module mkTB_All();

    BDPIIStream#(Bit#(32)) mt19937rng <- mkBDPIIStream(bsv_makeMersenneTwister19937);
    BDPITransformStream#(Tuple2#(UInt#(18),UInt#(4)),UInt#(19)) add18b4b <- mkBDPITransformStream(bsv_makeAdd_18b_4b);
//    BSVOStream#(Bit#(32)) 

    Action getMersenne = action
        let rnd <- mt19937rng.next.get;
        $display($time,": Next MT19937 value is %016X",rnd);
    endaction;

    function ActionValue#(UInt#(19)) testadd(UInt#(18) a,UInt#(4) b) = actionvalue
        let o <- add18b4b.fn(tuple2(a,b));
        $display($time,": Adder %d + %d = %d",a,b,o);
        return o;
    endactionvalue;

    function Action testadd_(UInt#(18) a,UInt#(4) b) = action
        let o <- testadd(a,b);
    endaction;

    Stmt stim = seq
        repeat(10)
            getMersenne;

        testadd_(1,1);
        testadd_(10,0);
        testadd_(0,10);
        testadd_(maxBound,0);
        testadd_(maxBound,maxBound);
        testadd_(0,maxBound);

        repeat(10) noAction;

    endseq;

    mkAutoFSM(stim);
endmodule

//module mkTB_mt19937();
//
//	let dut <- mkBDPIIStream(bsv_makeMersenneTwister19937);
//	PipeOut#(Bit#(32)) o <- mkSource_from_fav(dut.next.get);
//
//	let oFile <- mkBDPIOStream(bsv_makeOFileStreamP("mt19937.dut.out"));
//
//	let odisp <- mkTap(oFile.out.put,o);
//
//	Stmt stim = seq
//		repeat(1024) action
//			odisp.deq;
//			$display($time,": %08X",odisp.first);
//		endaction
//
//		repeat(10) noAction;
//	endseq;
//
//	mkAutoFSM(stim);
//endmodule
//
//module mkTB_IPipe();
//	let dut <- mkBDPIIStream(bsv_makeIFileStreamP("stim.in.txt"));
//	PipeOut#(Bit#(31)) iPipe <- mkSource_from_fav(dut.next.get);
//
//	Reg#(UInt#(16)) ctr <- mkReg(0);
//
//	Reg#(Bool) eof <- mkReg(False);
//
//	Stmt stim = seq
//
//		while (!eof) action
//			$display($time," Input: %08X",iPipe.first);
//			iPipe.deq;
//			ctr <= ctr+1;
//			let eofT <- dut.checkEndOfStream;
//			eof <= eofT;
//		endaction
//
//		dynamicAssert(ctr == 16,"Did not receive the expected number of outputs");
//	endseq;
//
//	mkAutoFSM(stim);
//
//endmodule


endpackage
