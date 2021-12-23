all: testExecuteStackOperation

testExecuteStackOperation: executeStackOperation.o test.cpp
	g++ -std=c++11 -o testExecuteStackOperation test.cpp executeStackOperation.o

executeStackOperation.o: executeStackOperation.c
	gcc -std=c99 -g -o executeStackOperation.o -c executeStackOperation.c

clean:
	rm -rf *.o testExecuteStackOperation