#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <string.h>
#include <sys/types.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <curses.h>
#include <termios.h>
#include <signal.h>
#include <sys/ioctl.h>

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


struct name_list
{
	mqd_t name_list_q;
	char list[4096];
} _users;

struct message_str
{
	mqd_t message_q;
	char message[350];
} user_message;

struct chat_str
{
	mqd_t chat_q;
	char chat[8192];
} _chat;

void send_name(char* name, mqd_t name_queue)
{
	input_name = newwin(6, 45, 12, 20);
	waddstr(input_name,"Enter your name: ");
    wgetstr(input_name, name);

	mq_send(name_queue, name, strlen(name), 0);

	wclear(input_name);
	wrefresh(input_name);
    delwin(input_name);
}

void* get_name_list(void* param)
{
	struct name_list* ptr;
	ptr = param;

	while (1)
	{
		curs_set(0);
		refresh();
		memset(ptr -> list, 0, 4096);
		mq_receive(ptr -> name_list_q, ptr -> list, 4096, NULL);

		if (strlen(ptr -> list) != 0)
		{
			werase(sub_users_win);
			wprintw(sub_users_win, ptr -> list);
    		wrefresh(sub_users_win);
		}
		refresh();
		sleep(1);
	}
}

void* get_chat(void* param)
{
	struct chat_str* ptr;
	ptr = param;
	int status;

	while (1)
	{
		status = mq_receive(ptr->chat_q, ptr->chat, 8192, NULL);
		
		if (status == -1)
		{
			continue;
		}
		
		curs_set(0);
		refresh();
		werase(sub_chat_win);
		wrefresh(sub_chat_win);
		wprintw(sub_chat_win,"%s", ptr->chat);
		wrefresh(sub_chat_win);
		wrefresh(users_win);
		wrefresh(input_win);
		refresh();
		sleep(1);
	}
}

void send_message(char* name, mqd_t message_queue)
{
	char message[325];
	char buf[300];

	echo();
	input_message = newwin(10, 60, 26, 1);
	waddstr(input_message, "Message: ");
	curs_set(TRUE);
	refresh();
    wgetstr(input_message, buf);

	sprintf(message, "[%s]: ", name);
	strncat(message, buf, strlen(buf));
	mq_send(message_queue, message, strlen(message), 0);
	
	wclear(input_message);
	wrefresh(input_message);
	wrefresh(sub_chat_win);
	wrefresh(users_win);
	wrefresh(sub_users_win);
	wrefresh(input_win);
    delwin(input_message);
	noecho();
	refresh();
}

int main()
{
	char name[20];
	pthread_t reciever_list_name, reciever_chat ;
	int item;
	mqd_t delete_q, name_q, message_q;

	name_q = mq_open("/name_queue", O_RDWR | O_NONBLOCK);
	_users.name_list_q = mq_open("/name_list_queue", O_RDWR | O_NONBLOCK);
	message_q = mq_open("/message_queue", O_RDWR | O_NONBLOCK);
	_chat.chat_q = mq_open("/chat_queue", O_RDWR | O_NONBLOCK);
	delete_q = mq_open("/delete_queue", O_RDWR | O_NONBLOCK);

	initscr();

    signal(SIGWINCH, sig_winch);
    curs_set(TRUE);
    refresh();

	send_name(name, name_q);

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

		
	pthread_create(&reciever_list_name, NULL, get_name_list, &_users);
	pthread_create(&reciever_chat, NULL, get_chat, &_chat);

	while (1)
	{
		keypad(input_win, TRUE);
        noecho();
    
        item = wgetch(input_win);
        switch (item)
        {
            case KEY_F(1):
                send_message(name, message_q);
				refresh();
                break;
            case KEY_F(2):
				mq_send(delete_q, name, strlen(name), 0);
				pthread_detach(reciever_list_name);
				pthread_detach(reciever_chat);
				curs_set(TRUE);
				refresh();
				endwin();
                exit(0);
        }
	}
}
