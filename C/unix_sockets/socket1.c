#include <sys/un.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#define BACKLOG 5
#define SIZE 100

int main(int argc, char *argv[])
{
    char *SOCK_NAME = "/tmp/mysock";
    int fd, cfd, w;
    ssize_t numRead;
    int s;
    char buf[SIZE];
    struct sockaddr_un addr;

    memset(buf, 0, SIZE);
    
    remove(SOCK_NAME);

    fd = socket(AF_UNIX, SOCK_STREAM, 0);

    memset(&addr, 0, sizeof(struct sockaddr_un));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SOCK_NAME, sizeof(addr.sun_path) - 1);

    s = bind(fd, (struct sockaddr *)&addr, sizeof(struct sockaddr_un));

    listen(fd, BACKLOG);

    while (1)
    {
        cfd = accept(fd, NULL, NULL);
        read(cfd, buf, SIZE);
        printf("%s\n", buf);
        memset(buf, 0, SIZE);
        close(cfd);
        
    }
}