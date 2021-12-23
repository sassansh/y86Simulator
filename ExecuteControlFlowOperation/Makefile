all: testExecuteControlFlowOperation

testExecuteControlFlowOperation: executeControlFlowOperation.o test.cpp
	g++ -std=c++11 -g -o testExecuteControlFlowOperation test.cpp executeControlFlowOperation.o

executeControlFlowOperation.o: executeControlFlowOperation.c
	gcc -std=c99 -o executeControlFlowOperation.o -c executeControlFlowOperation.c

clean:
	rm -rf *.o testExecuteControlFlowOperation