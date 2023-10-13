compile:
	gcc -c main.c -g
	gcc -c file.c -g
	gcc -c board.c -g
	gcc -c move.c -g
	gcc main.o file.o board.o move.o -o tileblaster -Wall -std=c99 -g

clean::
	rm *.o tileblaster
