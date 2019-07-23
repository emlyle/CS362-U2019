CFLAGS= -Wall -fpic -coverage -lm -std=c99

testme: testme.c 
	gcc -o testme testme.c -g $(CFLAGS)

runtest: testme
	./testme &> quiztestresult.out
	gcov -b -f testme.c >> quiztestresult.out
	cat testme.c.gcov >> quiztestresult.out

all: testme runtest

clean:
	rm -f *.o testme.exe testme runtest *.gcov *.gcda *.gcno *.so *.out 
