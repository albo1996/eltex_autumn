#include <curses.h>
#include <termios.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>
#include <fcntl.h>

void save_func(int);
int open_file(WINDOW*, WINDOW*);
void write_func(WINDOW*, int);
