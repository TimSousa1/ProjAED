debug::
	gcc -c main.c -pg
	gcc -c file.c -pg
	gcc -c board.c -pg
	gcc -c move.c -pg
	gcc main.o file.o board.o move.o -o tileblaster -Wall -std=c99 -O3 -pg

o3::
	gcc -c main.c 
	gcc -c file.c 
	gcc -c board.c
	gcc -c move.c
	gcc main.o file.o board.o move.o -o tileblaster -Wall -std=c99 -O3

clean::
	rm *.o tileblaster *.out vgcore* *.toleblists

run::
	./tileblaster ../tiles/Primeiro_milho/Primeiro_milho02.tilewalls
