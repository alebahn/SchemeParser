CFLAGS = -g -DDEBUG
LIBOPT = -lfl -ly
CC = gcc

default: schemeParser

schemeParser: scheme.tab.o scheme.o
	$(CC) -o schemeParser scheme.o scheme.tab.o $(LIBOPT)

scheme.tab.o: scheme.y
	bison -d scheme.y
	$(CC) -c scheme.tab.c

scheme.o: scheme.l
	flex -L -oscheme.c scheme.l
	$(CC) -c scheme.c

test: semantics.o testSemantics.o
	$(CC) $(CFLAGS) semantics.o testSemantics.o -o testSemantics
	./testSemantics

semantics.o: semantics.c semantics.h
	$(CC) $(CFLAGS) -c semantics.c

testSemantics.o: testSemantics.c testSemantics.h semantics.h
	$(CC) $(CFLAGS) -c testSemantics.c

clean:
	rm -f *.o
	rm -f testSemantics
	rm -f simpleCalc.c simpleCalc.tab.c simpleCalc.tab.h simpleCalc
