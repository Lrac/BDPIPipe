CXX_OPTS=-Wall -fPIC -O3 -g -std=c++11
LD_OPTS=-O3 -g -fPIC

unittest: Test_BDPIPipe_Unit
	./Test_BDPIPipe_Unit

Test_BDPIPipe_Unit: Test_BDPIPipe_Unit.o
	$(CXX) $(LD_OPTS) -o $@ $^ -lboost_unit_test_framework -lgmp
	
%.o: %.cpp *.hpp
	$(CXX) $(CXX_OPTS) -c $<

clean:
	rm -rf *.so *.o test model_*.c model_*.cxx *.out *.b[ao] *.dSYM mkTB_*.cxx mkTB_*.h vpi_wrapper* imported_BDPI_functions.h
