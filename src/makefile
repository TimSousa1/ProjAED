CFLAGS=-Wall -std=c99 -O3
ifdef PROF
	CFLAGS=-Wall -std=c99 -O3 -pg
endif

ifdef DEBUG
	CFLAGS=-Wall -std=c99 -g
endif

compile:
	gcc -c main.c $(CFLAGS)
	gcc -c file.c $(CFLAGS)
	gcc -c board.c $(CFLAGS)
	gcc -c move.c $(CFLAGS)
	gcc main.o file.o board.o move.o -o tileblaster $(CFLAGS)

clean::
	rm *.o tileblaster *.out vgcore* *.toleblists
