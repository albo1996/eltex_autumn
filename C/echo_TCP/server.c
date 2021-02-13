#include <sys/un.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <ctype.h>

#define BACKLOG 5
#define SIZE 128

int main()
{
    int i;
    int fd, cfd;
    ssize_t numBytes;
    char buf[SIZE];
    struct sockaddr_in addr;

    fd = socket(AF_INET, SOCK_STREAM, 0);

    addr.sin_family = AF_INET;
    addr.sin_port = htons(3200); 
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    bind(fd, (struct sockaddr *)&addr, sizeof(addr));

    listen(fd, BACKLOG);

    while (1)
    {
        cfd = accept(fd, NULL, NULL);

        numBytes = recv(cfd, buf, SIZE, 0);
        
        for(i = 0; i < numBytes; i++)
        {
            buf[i] = toupper((unsigned char) buf[i]);            
        }

        send(cfd, buf, SIZE, 0);

        memset(buf, 0, SIZE);

        close(cfd);
    }

}