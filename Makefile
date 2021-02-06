ls: main.o parse.o
	gcc -o ls main.o parse.o

main.o: main.c parse.o

parse.o: parse.h parse.c