#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <string.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>

void name_list()
{
    int fd;
    int size = 1024;

    fd = shm_open("name_list", O_RDWR | O_CREAT | O_EXCL, 0777);
	
	if(fd == -1)
	{
		printf("FD = %d\n", fd);
	}

    if (ftruncate(fd, size) == -1)
	{
		printf("ftruncate error!\n");
	}
}

void chat()
{
    int fd;
    int size = 4096;

    fd = shm_open("chat", O_RDWR | O_CREAT | O_EXCL, 0777);

    if(fd == -1)
	{
		printf("FD = %d\n", fd);
	}

    if (ftruncate(fd, size) == -1)
	{
		printf("ftruncate error!\n");
	}
}

int main()
{
    name_list();
    chat();
    sem_open("sem1", O_RDWR | O_CREAT | O_EXCL, 0777, 1);

    printf("Введите любой символ для завершения работы...\n");
    getchar();

    sem_unlink("sem1");
    shm_unlink("name_list");
    shm_unlink("chat");

    return 0;
}