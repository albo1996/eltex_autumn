#include <sys/un.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#include <pthread.h>

#define BACKLOG 5
#define SIZE 128
#define QUEUE_NAME "/clients"

int fd;
mqd_t mqd;

void* customer_service(void* param)
{
    int cfd;
    int len;
    ssize_t numBytes;
    char buf[SIZE];
    char message[2];
    struct sockaddr_in caddr;

    while (1)
    {    
        mq_receive(mqd, message, 2, NULL);
        cfd = (int) message[0];

        len = sizeof(caddr);

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
    int cfd;
    int len;
    char s[2];
    int flags = O_CREAT | O_EXCL | O_RDWR;
	mode_t mode = S_IRUSR | S_IWUSR;
	struct mq_attr mes_attr;
    struct sockaddr_in addr;
    struct sockaddr_in caddr;
    pthread_t t[10];

    mes_attr.mq_msgsize = 2;
	mes_attr.mq_maxmsg = 10;

    mq_unlink(QUEUE_NAME);

    mqd = mq_open(QUEUE_NAME, flags, mode, &mes_attr);
    
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

    while (1)
    {
        cfd = accept(fd, (struct sockaddr*)&caddr, &len);

        s[0] = (char) cfd;
        s[1] = 0;

        mq_send(mqd, s, sizeof(s), 0);
    }
}