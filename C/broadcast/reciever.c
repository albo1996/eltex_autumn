#include <sys/un.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h> 
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#define SIZE 64
#define ADDR "192.168.1.255"

int main()
{
    int fd;
    int len;
    char buf[SIZE];
    struct sockaddr_in saddr;

    fd = socket(AF_INET, SOCK_DGRAM, 0);

    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(3200);
    saddr.sin_addr.s_addr = inet_addr(ADDR);

	bind(fd, (struct sockaddr*)&saddr, sizeof(saddr));

    len = sizeof(struct sockaddr_in);

	while (1)
	{
		memset(buf, 0, SIZE);
		recvfrom(fd, buf, SIZE, 0, (struct sockaddr *) &saddr, &len);
    	printf("Response: %s\n", buf);	
	}
	
    
}