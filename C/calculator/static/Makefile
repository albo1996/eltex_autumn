all: libcalc_lib.a main.o
	gcc main.o -L. -lcalc_lib -o calc
libcalc_lib.a: add.o sub.o mul.o divi.o
	ar rc libcalc_lib.a add.o sub.o mul.o divi.o
add.o: add.c
	gcc -c add.c
sub.o: sub.c
	gcc -c sub.c
mul.o: mul.c
	gcc -c mul.c
divi.o: divi.c
	gcc -c divi.c
main.o: main.c
	gcc -c main.c
clean:
	rm -rf *.o

