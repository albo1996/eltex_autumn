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
    char string[SIZE];
    char buf[SIZE];
    struct sockaddr_in addr;

    printf("Введите строку...\n");
    fgets(string, SIZE, stdin);
    string[strlen(string) - 1] = 0;

    fd = socket(AF_INET, SOCK_STREAM, 0);

    addr.sin_family = AF_INET;
    addr.sin_port = htons(3200); 
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

    connect(fd, (struct sockaddr *)&addr, sizeof(addr));

    send(fd, string, SIZE, 0);

    recv(fd, buf, SIZE, 0);

    printf("Response: %s\n", buf);

    close(fd);

    return 0;
}