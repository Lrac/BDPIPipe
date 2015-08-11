package BDPIPipe;

import Assert::*;
import GetPut::*;
import StmtFSM::*;
import PAClib::*;
import FShow::*;

import RevertingVirtualReg::*;


/** Definition of an opaque object to hold a 64b pointer to a stream object
 */

typedef struct {
    UInt#(64) ptr;
} StreamHandle deriving(Bits,Eq);

StreamHandle streamHandleNull = StreamHandle { ptr: 0 };

instance FShow#(StreamHandle);
    function Fmt fshow(StreamHandle sh) = $format("[Stream*] %016X",sh.ptr);
endinstance



/** Module holds a 64b pointer, and returns it only if it's not null
 */

module mkHandleHolder#(ActionValue#(StreamHandle) f)(ReadOnly#(StreamHandle));
    Reg#(StreamHandle) sh <- mkReg(streamHandleNull);

    let openOnce <- mkOnce(action
        let t <- f;
        dynamicAssert(t != streamHandleNull,"Stream handle returned NULL");
        sh <= t;
    endaction);

    rule doOpen;
        openOnce.start;
    endrule

    method StreamHandle _read if (sh != streamHandleNull) = sh;
endmodule



// New polymorphic stream accessors (do not call directly - use wrappers below)
import "BDPI" bsv_writeStreamP = function Action bsv_writeStreamP(StreamHandle sh,UInt#(32) nb,ti d) provisos (Bits#(ti,ni));
import "BDPI" bsv_readStreamP  = function ActionValue#(to) bsv_readStreamP(StreamHandle sh,UInt#(32) nb) provisos (Bits#(to,no));
import "BDPI" bsv_readWriteStreamP = function ActionValue#(to) bsv_readWriteStreamP(StreamHandle sh,UInt#(32) nbo,UInt#(32) nbarg,ti i) provisos (Bits#(to,no),Bits#(ti,ni));

// Wrappers to pass in the appropriate bit lengths
function Action bsv_writeStream(StreamHandle sh,ti i) provisos (Bits#(ti,ni)) = bsv_writeStreamP(sh,fromInteger(valueOf(ni)),i);
function ActionValue#(to) bsv_readStream(StreamHandle sh) provisos (Bits#(to,no)) = bsv_readStreamP(sh,fromInteger(valueOf(no)));
function ActionValue#(to) bsv_readWriteStream(StreamHandle sh,ti i) provisos (Bits#(ti,ni),Bits#(to,no)) = bsv_readWriteStreamP(sh,fromInteger(valueOf(no)),fromInteger(valueOf(ni)),i);
import "BDPI" bsv_checkEndOfStream = function ActionValue#(Bool) bsv_checkEndOfStream(StreamHandle sh);


/** mkBDPIIStream
 *
 * Arguments:
 *  open        an ActionValue#(StreamHandle) which returns the stream object
 *
 * If both next.get and checkEndOfStream are called, next.get will happen first.
 *
 */


interface BDPIIStream#(type t);
    interface Get#(t)           next;
    method ActionValue#(Bool)   checkEndOfStream;
endinterface



module mkBDPIIStream#(ActionValue#(StreamHandle) open)(BDPIIStream#(to)) provisos (Bits#(to,no));
    let sh <- mkHandleHolder(open);

    Reg#(Bool) rvrReadFirstOrder <- mkRevertingVirtualReg(True);

    interface Get next;
        method ActionValue#(to) get if (rvrReadFirstOrder) = bsv_readStream(sh);
    endinterface

    method ActionValue#(Bool) checkEndOfStream;
        let eof <- bsv_checkEndOfStream(sh);
        rvrReadFirstOrder._write(False);            // enforces EOF checking to schedule after reading
        return eof;
    endmethod
endmodule



/** mkBDPIOStream
 *
 * Arguments:
 *  open        an ActionValue#(StreamHandle) which returns the stream object
 */

interface BDPIOStream#(type t);
    interface Put#(t)           out;
endinterface

module mkBDPIOStream#(ActionValue#(StreamHandle) open)(BDPIOStream#(ti)) provisos (Bits#(ti,ni));
    let sh <- mkHandleHolder(open);

    interface Put out;
        method Action put(ti i) = bsv_writeStream(sh,i);    // implicit condition: sh non-null
    endinterface
endmodule



/** mkBDPITransformStream
 *
 * Arguments:
 *  open        an ActionValue#(StreamHandle) which returns the stream object
 */


interface BDPITransformStream#(type ti,type to);
    method ActionValue#(to) fn(ti i);
endinterface

module mkBDPITransformStream#(ActionValue#(StreamHandle) open)(BDPITransformStream#(ti,to)) provisos (Bits#(ti,ni),Bits#(to,no));
    let sh <- mkHandleHolder(open);

    method ActionValue#(to) fn(ti i) = bsv_readWriteStream(sh,i);
endmodule

endpackage
