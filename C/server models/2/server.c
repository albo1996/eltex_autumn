#include <sys/un.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#include <pthread.h>

#define BACKLOG 10
#define SIZE 128

int fd;

void* customer_service(void* param)
{
    int cfd;
    int len;
    ssize_t numBytes;
    char buf[SIZE];
    struct sockaddr_in caddr;

    len = sizeof(caddr);

    while (1)
    {
        cfd = accept(fd, (struct sockaddr*)&caddr, &len);

        numBytes = recv(cfd, buf, SIZE, 0);
            
        for(int i = 0; i < numBytes; i++)
        {
            buf[i] = toupper((unsigned char) buf[i]);            
        }

        send(cfd, buf, SIZE, 0);

        memset(buf, 0, SIZE);

        close(cfd);
    }
}

int main()
{
    struct sockaddr_in addr;
    pthread_t t[10];
    
    fd = socket(AF_INET, SOCK_STREAM, 0);

    addr.sin_family = AF_INET;
    addr.sin_port = htons(3200); 
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    bind(fd, (struct sockaddr *)&addr, sizeof(addr));

    listen(fd, BACKLOG);

    for (int i = 0; i < 10; i++)
    {
        pthread_create(&t[i], NULL, customer_service, NULL);
    }

    getchar();
}
