
#include <stdlib.h>
#include <signal.h>
#include "graphics.h"
#include "file.h"

WINDOW *input_name;
WINDOW *mwnd;
WINDOW *inwnd;
WINDOW *sub_inwnd;
int fd;

void sig_winch(int signo)
{
    struct winsize size;
    ioctl(fileno(stdout), TIOCGWINSZ, (char *) &size);
    resizeterm(size.ws_row, size.ws_col);
}

int main()
{
    int item;

    initscr();
    signal(SIGWINCH, sig_winch);
    cbreak();
    color();
    curs_set(TRUE);
    refresh();

    inwnd = create_inwnd();
    sub_inwnd = create_subinwnd(inwnd);
    mwnd = create_mwnd();

    while (1)
    {
        keypad(mwnd, TRUE);
        noecho();
    
        item = wgetch(mwnd);
        switch (item)
        {
            case KEY_F(1):
                input_name = create_input_name(sub_inwnd);
                fd = open_file(input_name, sub_inwnd);
                clear_sub_inwnd(sub_inwnd);
                break;
            case KEY_F(2):
                write_func(sub_inwnd, fd);
                break;
            case KEY_F(3):
                save_func(fd);
                break;
            case KEY_F(4):
                close(fd);
                quit(sub_inwnd, inwnd, mwnd);
                exit(0);
        }
    }

    return 0;
}