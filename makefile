compile:
	gcc -c main.c -g -pg
	gcc -c file.c -g -pg
	gcc -c board.c -g -pg
	gcc -c move.c -g -pg
	gcc main.o file.o board.o move.o -o tileblaster -Wall -std=c99 -g -pg

clean::
	rm *.o tileblaster

run::
	./tileblaster ../tiles/Primeiro_milho/Primeiro_milho02.tilewalls
