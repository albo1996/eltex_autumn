#include <sys/un.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <ctype.h>

#define SIZE 64
#define ADDR "192.168.1.255"

int main()
{
	int param = 1;
    int len;
    int fd;
    char buf[SIZE];
    struct sockaddr_in saddr, caddr;

	memset(buf, 0, SIZE);

	printf("Enter your message\n");
	scanf("%s", buf);

    fd = socket(AF_INET, SOCK_DGRAM, 0);

    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(3200); 
    saddr.sin_addr.s_addr = inet_addr(ADDR);

	setsockopt(fd, SOL_SOCKET, SO_BROADCAST, &param, sizeof(param));
	len = sizeof(struct sockaddr_in);

    while (1)
    {
        sendto(fd, buf, SIZE, 0, (struct sockaddr *) &saddr, len);
		sleep(5);
    }
}