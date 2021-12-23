all: testFetchInstruction

testFetchInstruction: fetchInstruction.o test.cpp
	g++ -std=c++11 -g -o testFetchInstruction test.cpp fetchInstruction.o

fetchInstruction.o: fetchInstruction.c
	gcc -std=c99 -g -o fetchInstruction.o -c fetchInstruction.c

clean:
	rm -rf *.o testFetchInstruction