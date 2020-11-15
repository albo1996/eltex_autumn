all: main.o file.o graphics.o
	gcc -o main main.o file.o graphics.o -lncurses
main.o: main.c
	gcc -c main.c
file.o: file.c
	gcc -c file.c
graphics.o: graphics.c
	gcc -c graphics.c
clear:
	rm *.o
	rm main
