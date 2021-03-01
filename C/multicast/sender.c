#include <sys/un.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#define SIZE 64
#define ADDR "224.0.0.1"

int main()
{
	int param = 1;
    int len;
    int fd;
    char buf[SIZE];
    struct sockaddr_in saddr;
    struct ip_mreq mreq;

	memset(buf, 0, SIZE);

	printf("Enter your message\n");
	scanf("%s", buf);

    fd = socket(AF_INET, SOCK_DGRAM, 0);

    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(3200); 
    saddr.sin_addr.s_addr = inet_addr(ADDR);

    mreq.imr_multiaddr.s_addr = inet_addr(ADDR);
    mreq.imr_interface.s_addr = htonl(INADDR_ANY);

	setsockopt(fd, SOL_SOCKET, IP_ADD_MEMBERSHIP, (const void*)&mreq, sizeof(mreq));
	len = sizeof(struct sockaddr_in);

    while (1)
    {
        sendto(fd, buf, SIZE, 0, (struct sockaddr *) &saddr, len);
		sleep(5);
    }
}