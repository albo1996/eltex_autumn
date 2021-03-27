#include <sys/ioctl.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define MAJOR_NUM 0
#define IOCTL_GET_MSG _IOR(MAJOR_NUM, 1, char *)
#define IOCTL_SET_MSG _IOR(MAJOR_NUM, 0, char *)


int main()
{
    int fd;
    char buf[100];
    char answer[4];
    
    fd = open("/dev/RW_module", O_RDWR | O_NONBLOCK);

    printf("Message:\n");
    scanf("%s", buf);

    ioctl(fd, IOCTL_SET_MSG, buf);

    ioctl(fd, IOCTL_GET_MSG, answer);

    printf("Answer: %s\n", answer);


    close(fd);

}
