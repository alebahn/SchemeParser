CFLAGS = -g -DDEBUG

default: test

test: semantics.o testSemantics.o
	gcc $(CFLAGS) semantics.o testSemantics.o -o testSemantics
	./testSemantics

semantics.o: semantics.c semantics.h
	gcc $(CFLAGS) -c semantics.c

testSemantics.o: testSemantics.c testSemantics.h semantics.h
	gcc $(CFLAGS) -c testSemantics.c

clean:
	rm -f *.o
	rm -f testSemantics
