all: main.o _command_.o command_count.o param.o lexemes_count.o
	gcc main.o _command_.o command_count.o param.o lexemes_count.o -o main
main.o: main.c
	gcc -c main.c
_command_.o: _command_.c
	gcc -c _command_.c
command_count.o: command_count.c
	gcc -c command_count.c
param.o: param.c
	gcc -c param.c
lexemes_count.o: lexemes_count.c
	gcc -c lexemes_count.c
clear:
	rm main
	rm *.o