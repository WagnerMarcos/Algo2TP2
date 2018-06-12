CXXFLAGS = -g -Wall -Wpedantic -Wdeprecated -std=c++17 -O3
SRC = source
INCLUDE = include
TESTS = source/tests
GOOGLETEST =  source/tests/googletest
CXXARGS = -I. -iquote $(INCLUDE) -isystem $(GOOGLETEST)/include -pthread

all: fourier.exe Complex_test.exe fourier_test.exe

cmdline.o: $(SRC)/cmdline.cpp $(INCLUDE)/cmdline.h
	$(CXX) $(CXXFLAGS) $(CXXARGS) -c $(SRC)/cmdline.cpp -o cmdline.o

fourier.o: $(SRC)/fourier.cpp $(INCLUDE)/fourier.h $(INCLUDE)/Complex.h $(INCLUDE)/Vector.h
	$(CXX) $(CXXFLAGS) $(CXXARGS) -c $(SRC)/fourier.cpp -o fourier.o

io.o: $(SRC)/io.cpp $(INCLUDE)/io.h $(INCLUDE)/fourier.h $(INCLUDE)/Complex.h $(INCLUDE)/Vector.h
	$(CXX) $(CXXFLAGS) $(CXXARGS) -c $(SRC)/io.cpp -o io.o

process.o: $(SRC)/process.cpp $(INCLUDE)/io.h $(INCLUDE)/Complex.h $(INCLUDE)/Vector.h
	$(CXX) $(CXXFLAGS) $(CXXARGS) -c $(SRC)/process.cpp -o process.o

main.o: $(SRC)/main.cpp $(INCLUDE)/main.h $(INCLUDE)/process.h $(INCLUDE)/cmdline.h $(INCLUDE)/fourier.h
	$(CXX) $(CXXFLAGS) $(CXXARGS) -c $(SRC)/main.cpp -o main.o

fourier.exe: cmdline.o fourier.o io.o process.o main.o
	$(CXX) $(CXXFLAGS) $(CXXARGS) cmdline.o fourier.o  io.o process.o main.o -o fourier.exe

gtest-all.o:
	$(CXX) $(CXXFLAGS) -isystem $(GOOGLETEST)/include -I$(GOOGLETEST) -pthread -c $(GOOGLETEST)/src/gtest-all.cc -o gtest-all.o

Complex_test.o: $(TESTS)/Complex_test.cpp $(INCLUDE)/Complex_test.h $(INCLUDE)/Complex.h $(INCLUDE)/Vector.h
	$(CXX) $(CXXFLAGS) $(CXXARGS) -c $(TESTS)/Complex_test.cpp -o Complex_test.o

Complex_test.exe: gtest-all.o Complex_test.o cmdline.o
	$(CXX) $(CXXFLAGS) $(CXXARGS) gtest-all.o Complex_test.o cmdline.o -o Complex_test.exe

fourier_test.o: $(TESTS)/fourier_test.cpp $(INCLUDE)/fourier_test.h $(INCLUDE)/Complex.h $(INCLUDE)/Vector.h $(INCLUDE)/io.h $(INCLUDE)/cmdline.h $(INCLUDE)/fourier.h
	$(CXX) $(CXXFLAGS) $(CXXARGS) -c $(TESTS)/fourier_test.cpp -o fourier_test.o

fourier_test.exe: fourier_test.o gtest-all.o cmdline.o
	$(CXX) $(CXXFLAGS) $(CXXARGS) gtest-all.o fourier_test.o fourier.o cmdline.o io.o -o fourier_test.exe

clean:
	$(RM) -vf *.o *.exe *.t *.out *.err
