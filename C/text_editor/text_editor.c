#include <termios.h>
#include <sys/ioctl.h>
#include <signal.h>
#include <stdlib.h>
#include <curses.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int fd;
char buf[1024];
WINDOW *input_name;
WINDOW *mwnd;
WINDOW *inwnd;
WINDOW *sub_inwnd;

void sig_winch(int signo)
{
    struct winsize size;
    ioctl(fileno(stdout), TIOCGWINSZ, (char *) &size);
    resizeterm(size.ws_row, size.ws_col);
}

void save_func()
{
    lseek(fd, SEEK_SET, 0);
    write(fd, buf, strlen(buf));
    close(fd);
}

void open_func()
{
    char file_name[128];
    char text[1024];

    wrefresh(sub_inwnd);
    echo();

    input_name = newwin(6, 45, 12, 20);
    wattron(input_name, COLOR_PAIR(2));
    wattron(input_name, A_BOLD);

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

    wrefresh(sub_inwnd);
    wclear(sub_inwnd);
}

void write_func()
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


void quit()
{
    close(fd);
    delwin(sub_inwnd);
    delwin(inwnd);
    delwin(mwnd);
    refresh();
    endwin();
    exit(EXIT_SUCCESS);
}

int main()
{
    int item;

    initscr();
    signal(SIGWINCH, sig_winch);
    cbreak();
    start_color();
    init_pair(1, COLOR_BLACK, COLOR_GREEN);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    curs_set(TRUE);
    refresh();
   
    inwnd = newwin(20, 80, 0, 0);
    wattron(inwnd, COLOR_PAIR(2));
    wattron(inwnd, A_BOLD);
    box(inwnd, '|', '-');
    wrefresh(inwnd);

    sub_inwnd = derwin(inwnd, 18, 78, 1, 1);
    
    mwnd = newwin(5, 60, 20, 2);
    wattron(mwnd, COLOR_PAIR(1));
    wattron(mwnd, A_BOLD);
    wprintw(mwnd, "   F1 - Open  F2 - Write  F3 - Save  F4 - Exit\t");
    wrefresh(mwnd);
    
    while (1)
    {
        keypad(mwnd, TRUE);
        noecho();
    
        item = wgetch(mwnd);
        switch (item)
        {
            case KEY_F(1):
                open_func();
                break;
            case KEY_F(2):
                write_func();
                break;
            case KEY_F(3):
                save_func();
                break;
            case KEY_F(4):
                quit();
        }
    }

    return 0;
}