all: testExecuteSimpleOperation

testExecuteSimpleOperation: executeSimpleOperation.o test.cpp
	g++ -std=c++11 -o testExecuteSimpleOperation test.cpp executeSimpleOperation.o

executeSimpleOperation.o: executeSimpleOperation.c
	gcc -std=c99 -o executeSimpleOperation.o -c executeSimpleOperation.c

clean:
	rm -rf *.o testExecuteSimpleOperation