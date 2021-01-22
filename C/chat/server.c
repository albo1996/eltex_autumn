#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <string.h>
#include <sys/types.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>


void get_name(char* name, char* name_list, mqd_t name_queue)
{
	int status;
	char buf[20];

	memset(buf, 0, 20);

	while (1)
	{
		status = mq_receive(name_queue, buf, 20, NULL);
		if (status != -1)
		{
			sprintf(name, "%s\n", buf);
			strncat(name_list, name, strlen(name));
		}
		else
		{
			break;
		}
	}
}

int users_number(char* name_list)
{
	int count = 0;

	for (int i = 0; i <= strlen(name_list); i++)
	{
		if (name_list[i] == '\n')
		{
			count++;
		}
	}
	return count;
}

void send_name_list(char* name_list, int users_number, mqd_t name_list_queue)
{
	int status;
	if (users_number > 0)
	{
		for (int i = 0; i < users_number; i++)
		{
			status = mq_send(name_list_queue, name_list, strlen(name_list), 0);
		}
	}
}

void get_message(char* message, char* chat, mqd_t message_queue)
{
	int status;
	char buf[350];

	memset(buf, 0, 350);

	while (1)
	{
		status = mq_receive(message_queue, buf, 8192, NULL);
		if (status != -1)
		{
			sprintf(message, "%s\n", buf);
			strncat(chat, message, strlen(message));
		}
		else
		{
			break;
		}
	}
}

void send_chat(char* chat, int users_number, mqd_t chat_queue)
{
	int status;
	if (users_number > 0)
	{
		for (int i = 0; i < users_number; i++)
		{
			status = mq_send(chat_queue, chat, strlen(chat), 0);
		}
	}
}
void delete_name(char* name_list, mqd_t delete_queue)
{
	char* ptr;
    char buf[4096];
    char name[20];

    memset(name, 0, 20);
    memset(buf, 0, 4096);

    strcpy(buf, name_list);
    strcat(buf, "\0");

    memset(name_list, 0, 4096);

    mq_receive(delete_queue, name, 20, NULL);

    ptr = strtok(buf, "\n");

    while (ptr != NULL)
    {
        if (strcmp(name, ptr) != 0)
        {
            strncat(name_list, ptr, strlen(ptr));
            strcat(name_list, "\n\0");
            ptr = strtok(NULL, "\n");
        }
        else
        {
            ptr = strtok(NULL, "\n");
        }
    }
}

int main()
{
	char name[20];
	char name_list[4096];
	char message[350];
	char chat[8192];
	int users;
	mqd_t name_q, name_list_q, message_q, chat_q, delete_q;
   	int flags = O_CREAT | O_EXCL | O_RDWR | O_NONBLOCK;
	mode_t mode = S_IRUSR | S_IWUSR;
	struct mq_attr attr_name;
	struct mq_attr attr_name_list;
	struct mq_attr attr_message;
	struct mq_attr attr_chat;
	struct mq_attr attr_delete;

	attr_name.mq_maxmsg = 10;
	attr_name.mq_msgsize = 20;

	attr_name_list.mq_maxmsg = 10;
	attr_name_list.mq_msgsize = 4096;

	attr_delete.mq_maxmsg = 10;
	attr_delete.mq_msgsize = 20;

	mq_unlink("/name_queue");
	mq_unlink("/name_list_queue");
	mq_unlink("/message_queue");
    mq_unlink("/chat_queue");
	mq_unlink("/delete_queue");

	name_q = mq_open("/name_queue", flags, mode, &attr_name);
	name_list_q = mq_open("/name_list_queue", flags, mode, &attr_name_list);
	message_q = mq_open("/message_queue", flags, mode, NULL);
  	chat_q = mq_open("/chat_queue", flags, mode, NULL);
	delete_q = mq_open("/delete_queue", flags, mode, &attr_delete);

	while (1)
	{
		get_name(name, name_list, name_q);

		users = users_number(name_list);

		send_name_list(name_list, users, name_list_q);

		get_message(message, chat, message_q);

		send_chat(chat, users, chat_q);

		delete_name(name_list, delete_q);
	}
}
