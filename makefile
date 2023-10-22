debug::
	gcc -c main.c -g -pg
	gcc -c file.c -g -pg
	gcc -c board.c -g -pg
	gcc -c move.c -g -pg
	gcc main.o file.o board.o move.o -o tileblaster -Wall -std=c99 -g -pg

o3::
	gcc -c main.c 
	gcc -c file.c 
	gcc -c board.c
	gcc -c move.c
	gcc main.o file.o board.o move.o -o tileblaster -Wall -std=c99 -O3

clean::
	rm *.o tileblaster

run::
	./tileblaster ../tiles/Primeiro_milho/Primeiro_milho02.tilewalls
