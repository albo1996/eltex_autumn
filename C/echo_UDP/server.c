#include <sys/un.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <ctype.h>

#define SIZE 128

int main()
{
    int i;
    int len;
    int fd;
    ssize_t numBytes;
    char buf[SIZE];
    struct sockaddr_in saddr, caddr;

    fd = socket(AF_INET, SOCK_DGRAM, 0);

    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(3200); 
    saddr.sin_addr.s_addr = htonl(INADDR_ANY);

    bind(fd, (struct sockaddr *)&saddr, sizeof(struct sockaddr_in));

    while (1)
    {
        len = sizeof(struct sockaddr_in);
        numBytes = recvfrom(fd, buf, SIZE, MSG_WAITALL, (struct sockaddr *) &caddr, &len);
        for (i = 0; i < numBytes; i++)
        {
            buf[i] = toupper((unsigned char) buf[i]);
        }

        sendto(fd, buf, numBytes, 0, (struct sockaddr *) &caddr, len);
        memset(buf, 0, SIZE);
    }
}