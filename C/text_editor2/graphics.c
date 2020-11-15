#include "graphics.h"

void quit(WINDOW *sub_inwnd, WINDOW *inwnd, WINDOW *mwnd)
{
    
    delwin(sub_inwnd);
    delwin(inwnd);
    delwin(mwnd);
    refresh();
    endwin();
}

WINDOW* create_inwnd()
{
    WINDOW *inwnd;
    inwnd = newwin(20, 80, 0, 0);
    wattron(inwnd, COLOR_PAIR(2));
    wattron(inwnd, A_BOLD);
    box(inwnd, '|', '-');
    wrefresh(inwnd);

    return inwnd;
}

WINDOW* create_subinwnd(WINDOW *inwnd)
{   
    WINDOW *sub_inwnd;
    sub_inwnd = derwin(inwnd, 18, 78, 1, 1);
    
    return sub_inwnd;
}

WINDOW* create_mwnd()
{
    WINDOW *mwnd;
    mwnd = newwin(5, 60, 20, 2);
    wattron(mwnd, COLOR_PAIR(1));
    wattron(mwnd, A_BOLD);
    wprintw(mwnd, "   F1 - Open  F2 - Write  F3 - Save  F4 - Exit\t");
    wrefresh(mwnd);

    return mwnd;
}

WINDOW* create_input_name(WINDOW *sub_inwnd)
{
    WINDOW *input_name;
    wrefresh(sub_inwnd);
    echo();
    input_name = newwin(6, 45, 12, 20);
    wattron(input_name, COLOR_PAIR(2));
    wattron(input_name, A_BOLD);

    return input_name;

}



void clear_sub_inwnd(WINDOW *sub_inwnd)
{
    wrefresh(sub_inwnd);
    wclear(sub_inwnd);
}

void color()
{
    start_color();
    init_pair(1, COLOR_BLACK, COLOR_GREEN);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
}