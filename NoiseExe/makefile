ROOTINC = $(shell root-config --cflags) 
ROOTLIBS = $(shell root-config --glibs)

all: LBNoise

LBnoise.o : LBnoise.cc
	g++ -c -Wall LBnoise.cc $(ROOTINC)

LBName.o : LBName.cc
	g++ -c -Wall LBName.cc

LBNoise : LBnoise.o LBName.o
	g++ main.cpp -o LBNoise LBnoise.o LBName.o $(ROOTLIBS)

clean:
	rm -rf *.o *~ 
	mv *.txt *.root results
