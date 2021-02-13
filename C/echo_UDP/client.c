#include <sys/un.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#define SIZE 128

int main()
{
    int fd;
    socklen_t len;
    char string[SIZE];
    char buf[SIZE];
    struct sockaddr_in saddr;

    printf("Введите строку...\n");
    fgets(string, SIZE, stdin);
    buf[strlen(string) - 1] = 0;

    fd = socket(AF_INET, SOCK_DGRAM, 0);

    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(3200);
    saddr.sin_addr.s_addr = htonl(INADDR_ANY);

    len = sizeof(struct sockaddr_in);

    sendto(fd, string, SIZE, 0, (struct sockaddr *) &saddr, len);

    recvfrom(fd, buf, SIZE, MSG_WAITALL, (struct sockaddr *) &saddr, &len);

    printf("Response: %s\n",buf);

    close(fd);
}