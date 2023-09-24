compile:
	gcc -c main.c -g
	gcc -c read.c -g
	gcc main.o read.o -o tileblaster -Wall -Wextra -g

clean::
	rm *.o tileblaster
