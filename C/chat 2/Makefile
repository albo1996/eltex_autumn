all: server.c client.c
	gcc server.c -o server -lrt -lpthread
	gcc client.c -o client -lrt -lpthread -lncurses
clear:
	rm server
	rm client
