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

int client_count = 0;

void* customer_service(void* param)
{
    int *cfd = (int*)param;
    ssize_t numBytes;
    char buf[SIZE];

    numBytes = recv(*cfd, buf, SIZE, 0);
        
    for(int i = 0; i < numBytes; i++)
    {
        buf[i] = toupper((unsigned char) buf[i]);            
    }

    send(*cfd, buf, SIZE, 0);

    memset(buf, 0, SIZE);

    close(*cfd);

    client_count--;
    
}

int main()
{
    int i;
    int len;
    int fd, cfd;
    int cfds[10];
    struct sockaddr_in addr;
    struct sockaddr_in caddr;
    pthread_t t[10];
    
    len = sizeof(caddr);

    fd = socket(AF_INET, SOCK_STREAM, 0);

    addr.sin_family = AF_INET;
    addr.sin_port = htons(3200); 
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    bind(fd, (struct sockaddr *)&addr, sizeof(addr));

    listen(fd, BACKLOG);

    while (1)
    {
        cfd = accept(fd, (struct sockaddr*)&caddr, &len);

        client_count++;

        if (client_count > 10)
        {
            printf("Max client count");
			exit(EXIT_FAILURE);
        }

        cfds[client_count - 1] = cfd;
        pthread_create(&t[client_count - 1], NULL, customer_service, &cfds[client_count - 1]);
    }
}