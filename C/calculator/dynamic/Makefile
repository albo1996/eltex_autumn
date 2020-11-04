p := plugins
s := src

all: main.c $(p)/libadd.so $(p)/libsub.so $(p)/libmul.so $(p)/libdivi.so
	gcc main.c -o main -ldl && rm *.o
$(p)/libadd.so: add.o
	gcc -shared add.o -o $(p)/libadd.so
$(p)/libsub.so: sub.o
	gcc -shared sub.o -o $(p)/libsub.so
$(p)/libmul.so: mul.o
	gcc -shared mul.o -o $(p)/libmul.so
$(p)/libdivi.so: divi.o
	gcc -shared divi.o -o $(p)/libdivi.so
add.o: src/add.c
	mkdir plugins && gcc -fPIC -c $(s)/add.c
sub.o: src/sub.c
	gcc -fPIC -c $(s)/sub.c
mul.o: src/mul.c
	gcc -fPIC -c $(s)/mul.c
divi.o: src/divi.c
	gcc -fPIC -c $(s)/divi.c

	
	
