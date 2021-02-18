#include <sys/un.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#include <pthread.h>

#define BACKLOG 5
#define SIZE 128

int fd[5];

void* customer_service(void* param)
{
    int cfd[5];
    int len;
    ssize_t numBytes;
    char buf[SIZE];
    struct sockaddr_in caddr;
    struct timeval tv;
    fd_set readfds;
    int ret;

    while (1)
    {
        tv.tv_sec = 2;
        tv.tv_usec = 0;

        FD_ZERO(&readfds);

        for (int i = 0; i < 5; i++)
        {
            FD_SET(fd[i], &readfds); 
        }

        ret = select(fd[4] + 1, &readfds, NULL, NULL, &tv);
        
        if (ret > 0)
        {
            for (int i = 0; i < 5; i++)
            {
                if (FD_ISSET(fd[i], &readfds) > 0)
                {
                    len = sizeof(caddr);
                    cfd[i] = accept(fd[i], (struct sockaddr*)&caddr, &len);
                    numBytes = recv(cfd[i], buf, SIZE, 0);
                        
                    for(int i = 0; i < numBytes; i++)
                    {
                        buf[i] = toupper((unsigned char) buf[i]);            
                    }

                    send(cfd[i], buf, SIZE, 0);

                    memset(buf, 0, SIZE);

                    close(cfd[i]);
                }
            }   
        }    
    }
}

int create_socket(int i)
{
    int fd;
    int port = 3200 + i;
    struct sockaddr_in addr;

    fd = socket(AF_INET, SOCK_STREAM, 0);

    addr.sin_family = AF_INET;
    addr.sin_port = htons(port); 
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    bind(fd, (struct sockaddr *)&addr, sizeof(addr));

    listen(fd, BACKLOG);
    
    return fd;
}

int main()
{
    int len;
    struct sockaddr_in caddr;
    pthread_t t[10];

    for (int i = 0; i < 5; i++)
    {
        fd[i] = create_socket(i);
    }
    
    for (int i = 0; i < 10; i++)
    {
        pthread_create(&t[i], NULL, customer_service, NULL);
    }

    getchar();
}






