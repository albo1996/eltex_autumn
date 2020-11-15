#include "file.h"


char buf[1024];

void save_func(int fd)
{
    lseek(fd, SEEK_SET, 0);
    write(fd, buf, strlen(buf));
    close(fd);
}

int open_file(WINDOW *input_name, WINDOW *sub_inwnd)
{   int fd;
    char file_name[128];
    char text[1024];

    waddstr(input_name,"Enter the file name: ");
    wgetstr(input_name, file_name);

    noecho();
    wclear(input_name);
    wrefresh(input_name);
    delwin(input_name);

    fd = open(file_name, O_RDWR);
    if (fd == -1)
    {
        waddstr(sub_inwnd,"File not found! Try again.");
    }
    else
    {
        read(fd, text, 1024);
        waddstr(sub_inwnd, text);
    }
    return fd;
}


void write_func(WINDOW *sub_inwnd, int fd)
{
    wclear(sub_inwnd);
    wrefresh(sub_inwnd);
    
    if (fd <= 0)
    {
        waddstr(sub_inwnd,"Open a file to write.");
        wrefresh(sub_inwnd);

    }
    else
    {
        move(0,0);
        echo();
        wgetstr(sub_inwnd, buf);
    } 

    wclear(sub_inwnd);
}