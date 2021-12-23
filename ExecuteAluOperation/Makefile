all: testExecuteAluOperation

testExecuteAluOperation: executeAluOperation.o test.cpp
	g++ -std=c++11 -g -o testExecuteAluOperation test.cpp executeAluOperation.o

executeAluOperation.o: executeAluOperation.c
	gcc -std=c99 -g -o executeAluOperation.o -c executeAluOperation.c

clean:
	rm -rf *.o testExecuteAluOperation