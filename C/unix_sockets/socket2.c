#include <sys/un.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>

#define SIZE 100


int main(int argc, char *argv[])
{
    extern int errno;
    char *SOCK_NAME = "/tmp/mysock";
    int fd, cfd, s, w;
    ssize_t numRead;
    char buf[SIZE];
    struct sockaddr_un addr;


    fgets(buf, SIZE, stdin);
    buf[strlen(buf) - 1] = 0;

    fd = socket(AF_UNIX, SOCK_STREAM, 0);
    printf("SOCKET = %d\n", fd);

    memset(&addr, 0, sizeof(struct sockaddr_un));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SOCK_NAME, sizeof(addr.sun_path) - 1);

    s = connect(fd, (struct sockaddr *) &addr, sizeof(struct sockaddr_un));
    printf("%s\n", strerror(errno));
    printf("CONNECT = %d\n", s);
   
        w = write(fd, buf, strlen(buf));
        printf("WRITE = %d\n", w);
    
}