#include <curses.h>
#include <termios.h>

void quit(WINDOW*, WINDOW*, WINDOW*);
WINDOW* create_inwnd();
WINDOW* create_subinwnd(WINDOW*);
WINDOW* create_mwnd();
WINDOW* create_input_name(WINDOW*);
void clear_sub_inwnd(WINDOW*);
void color();
