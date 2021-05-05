#include <sys/un.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>

#define SIZE 100

int main()
{
    char *SOCK_NAME = "/tmp/server";
    int fd, cfd, s, w;
    ssize_t numBytes;
    size_t msgLen;
    char buf[SIZE];
    char resp[SIZE];
    struct sockaddr_un svaddr, claddr;;

    fd = socket(AF_UNIX, SOCK_DGRAM, 0);

    memset(&claddr, 0, sizeof(struct sockaddr_un));
    claddr.sun_family = AF_UNIX;
    sprintf(claddr.sun_path,"/tmp/server.%ld", (long) getpid());
    bind(fd, (struct sockaddr *) &claddr, sizeof(struct sockaddr_un));

    memset(&svaddr, 0, sizeof(struct sockaddr_un));
    svaddr.sun_family = AF_UNIX;
    strncpy(svaddr.sun_path, SOCK_NAME, sizeof(svaddr.sun_path) - 1);

    fgets(buf, SIZE, stdin);
    buf[strlen(buf) - 1] = 0;

    sendto(fd, buf, strlen(buf), 0, (struct sockaddr *) &svaddr, sizeof(struct sockaddr_un));

    recvfrom(fd, resp, SIZE, 0, NULL, NULL);

    printf("Response: %s\n",resp);
    remove(claddr.sun_path);
    exit(EXIT_SUCCESS);

}