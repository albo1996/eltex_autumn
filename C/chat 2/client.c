#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <string.h>
#include <sys/types.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <curses.h>
#include <termios.h>
#include <signal.h>
#include <sys/ioctl.h>
#include <semaphore.h>

WINDOW* chat_win;
WINDOW* users_win;
WINDOW* input_win;
WINDOW* input_name;
WINDOW* sub_users_win;
WINDOW* sub_chat_win;
WINDOW* input_message;

void sig_winch(int signo)
{
	struct winsize size;
    	ioctl(fileno(stdout), TIOCGWINSZ, (char *) &size);
    	resizeterm(size.ws_row, size.ws_col);
}

void send_name(char* name, sem_t* sem)
{
	int fd;
    	int len;
    	char *addr;

	input_name = newwin(6, 45, 12, 20);
	waddstr(input_name,"Enter your name: ");
	wgetstr(input_name, name);

    	len = strlen(name);
    	sem_wait(sem);
	fd = shm_open("name_list", O_RDWR, 0);
	addr = mmap(NULL, len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	close(fd);
	strcat(addr, name);
	strcat(addr, "\n");

	wclear(input_name);
	wrefresh(input_name);
	delwin(input_name);
	refresh();
	sem_post(sem);
}

void send_message(char* name)
{
	int fd;
    	int len;
    	char *addr;
    	char message[128];
    	char buf[158];

    	echo();
	input_message = newwin(10, 60, 26, 1);
	waddstr(input_message, "Message: ");
	curs_set(TRUE);
	refresh();
    	wgetstr(input_message, message);

    	sprintf(buf, "[%s]: %s\n", name, message);
    	len = strlen(buf);
    	fd = shm_open("chat", O_RDWR, 0);
    	addr = mmap(NULL, len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    	close(fd);
    	strcat(addr, buf);

    	wclear(input_message);
	wrefresh(input_message);
	wrefresh(sub_chat_win);
	wrefresh(users_win);
	wrefresh(sub_users_win);
	wrefresh(input_win);
    	delwin(input_message);
	noecho();
    	curs_set(FALSE);
	refresh();
}

void* get_name_list(void* param)
{
    int fd;
    char* addr;
    struct stat sb;
    
    curs_set(FALSE);
    refresh();

    fd = shm_open("name_list", O_RDONLY, 0);
    fstat(fd, &sb);
    addr = mmap(NULL, sb.st_size, PROT_READ, MAP_SHARED, fd, 0);
    close(fd);

    while(1)
    {
	    curs_set(FALSE);
	    refresh();
	    werase(sub_users_win);
	    wprintw(sub_users_win, "%s", addr);
	    wrefresh(sub_users_win);
	    wrefresh(users_win);
	    sleep(1);
    }
}

void* get_chat(void* param)
{
    int fd;
    char* addr;
    struct stat sb;

    curs_set(FALSE);
    refresh();

    fd = shm_open("chat", O_RDONLY, 0);
    fstat(fd, &sb);
    addr = mmap(NULL, sb.st_size, PROT_READ, MAP_SHARED, fd, 0);
    close(fd);

    while(1)
    {
        curs_set(FALSE);
        werase(sub_chat_win);
	wrefresh(sub_chat_win);
        refresh();
	wprintw(sub_chat_win,"%s", addr);
	wrefresh(sub_chat_win);
	wrefresh(users_win);
	wrefresh(input_win);
        refresh();
    	sleep(1);
    }
}

void delete_name(char* name, sem_t* sem)
{
	int fd;
	char* addr;
    	char* ptr;
    	char buf[4096];
    	char name_list[4096];
    	struct stat sb;

    	memset(name_list, 0, 4096);

    	sem_wait(sem);
    	fd = shm_open("name_list", O_RDWR, 0);
    	fstat(fd, &sb);
    	addr = mmap(NULL, sb.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    	close(fd);
    	strncat(buf, addr, strlen(addr));

    	ptr = strtok(buf, "\n");

    	while (ptr != NULL)
    	{
        	if (strcmp(name, ptr) != 0)
        {
            strncat(name_list, ptr, strlen(ptr));
            strcat(name_list, "\n");
            ptr = strtok(NULL, "\n");
        }
        else
        {
            ptr = strtok(NULL, "\n");
        }
    }
    memset(addr, 0, sb.st_size);
    strncat(addr, name_list, strlen(name_list));
    sem_post(sem);
}

void graphics()
{
	curs_set(FALSE);
	refresh();

    	chat_win = newwin(25, 60, 0, 0);

    	box(chat_win, 0, 0);

    	wrefresh(chat_win);

	sub_chat_win = derwin(chat_win, 23, 58, 1, 1);

	wrefresh(sub_chat_win);

    	users_win = newwin(25, 25, 0, 62);
    	box(users_win, 0, 0);
    	wrefresh(users_win);

	sub_users_win = derwin(users_win, 23, 23, 1, 1);
	wrefresh(sub_users_win);

    	input_win = newwin(1, 35, 26, 58);
	wprintw(input_win, "   F1 - Wrie F2 - Exit\t");
    	wrefresh(input_win);
    	refresh();
}

int main()
{
	int item;
    	char name[25];
    	sem_t* sem;
    	pthread_t reciever_list_name, reciever_chat;
    
    	sem = sem_open("sem1", O_RDWR);

    	initscr();

    	signal(SIGWINCH, sig_winch);
    	curs_set(TRUE);
    	refresh();

	send_name(name, sem);

    	graphics();
	
    	pthread_create(&reciever_list_name, NULL, get_name_list, NULL);
	pthread_create(&reciever_chat, NULL, get_chat, NULL);

while (1)
{
	keypad(input_win, TRUE);
        noecho();
    
        item = wgetch(input_win);
        switch (item)
        {
            case KEY_F(1):
                send_message(name);
                break;
            case KEY_F(2):
				pthread_detach(reciever_list_name);
				pthread_detach(reciever_chat);
                		delete_name(name, sem);
				curs_set(TRUE);
				refresh();
				endwin();
                exit(0);
        }
    }
}
