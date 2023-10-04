compile:
	gcc -c main.c 
	gcc -c file.c
	gcc -c board.c
	gcc main.o file.o board.o -o tileblaster -Wall -std=c99 -O3

clean::
	rm *.o tileblaster
