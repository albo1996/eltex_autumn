all: main.o random_number.o loading.o purchases.o
	gcc main.o random_number.o loading.o purchases.o -o main -lpthread
main.o: main.c
	gcc -c main.c
random_number.o: random_number.c
	gcc -c random_number.c
loading.o: loading.c
	gcc -c loading.c
purchases.o: purchases.c
	gcc -c purchases.c
clear:
	rm *.o
	rm main
