all: server.c client.c
	gcc server.c -o server -lpthread -lrt
	gcc client.c -o client
clear:
	rm server
	rm client
