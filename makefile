compile:
	gcc -c main.c -g
	gcc -c read.c -g
	gcc -c board.c -g
	gcc main.o read.o board.o -o tileblaster -Wall -std=c99 -g

clean::
	rm *.o tileblaster
