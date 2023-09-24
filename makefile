compile:
	gcc -c main.c
	gcc -c read.c
	gcc main.o read.o -o tileblaster -Wall -Wextra -g

clean::
	rm *.o tileblaster
