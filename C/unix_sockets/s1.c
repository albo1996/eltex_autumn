#include <sys/un.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>

#define BACKLOG 5
#define SIZE 100

int main()
{
    char *SOCK_NAME = "/tmp/server";
    int fd, cfd;
    ssize_t numBytes;
    int len;
    int j;
    char buf[SIZE];
    struct sockaddr_un svaddr, claddr;

    fd = socket(AF_UNIX, SOCK_DGRAM, 0);

    remove(SOCK_NAME);

    memset(&svaddr, 0, sizeof(struct sockaddr_un));
    svaddr.sun_family = AF_UNIX;
    strncpy(svaddr.sun_path, SOCK_NAME, sizeof(svaddr.sun_path) - 1);

    bind(fd, (struct sockaddr *)&svaddr, sizeof(struct sockaddr_un));

    while (1)
    {
        len = sizeof(struct sockaddr_un);
        numBytes = recvfrom(fd, buf, SIZE, 0, (struct sockaddr *) &claddr, &len);
        printf("%s\n", buf);
        for (j = 0; j < numBytes; j++)
        {
            buf[j] = toupper((unsigned char) buf[j]);
        }

        sendto(fd, buf, numBytes, 0, (struct sockaddr *) &claddr, len);
        memset(buf, 0, SIZE);
    }
    

    
}