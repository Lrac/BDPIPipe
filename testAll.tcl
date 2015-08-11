vsim -pli libBDPIPipe_VPI32.so mkTB_All
force -drive CLK 1'b0, 1'b1 5 -repeat 10
force -drive RST_N 1'b0, 1'b1 10
onfinish stop
run -all
