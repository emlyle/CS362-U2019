CFLAGS= -Wall -fpic -coverage -lm -std=c99

testme: testme.c
	gcc -o testme.c -g $(CFLAGS)

runtest: testme
	./testme &> quiztestresult.out
	gcov testme.c >> quiztestresult.out
	cat testme.c.gcov -b -f >> quiztestresult.out

all: testme 

clean:
	rm -f *.o testme.exe testme *.gcov *.gcda *.gcno *.so *.out 
