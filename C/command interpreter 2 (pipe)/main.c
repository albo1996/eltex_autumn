#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include "lexemes_count.h"
#include "param.h"
#include "command_count.h"
#include "_command_.h"


int main()
{
	pid_t pid;
	int i, j;
	int p_count;
	int l_count;
	int cmp;
	int pfd[2];
    	extern int errno;
    	char command[512];
	char *command_path[10];
    	char *argv[256];
    	char path[]="/bin/";
    
	
	while(1)
    	{
    		printf("$ ");
		fgets(command, 50, stdin);
		i = strlen(command) - 1;
		if (command[i] == '\n')
		{
			command[i]='\0';
		}
	
		cmp = strcmp("exit", command);
    		if (cmp == 0)
    		{
    			exit(0);
    		}

		p_count = command_count(command);

		_command_ (command, command_path, p_count);

		pipe(pfd);

		for (j = 0; j < p_count; j++)
		{
			l_count = lexemes_count(command_path[j]);

			param(argv, command_path[j], path, l_count);

			pid = fork();
			
			if (pid == 0)
			{	
				if(((j%2) == 0) && (p_count > 1))
				{
					close(pfd[0]);
					dup2(pfd[1], STDOUT_FILENO);
					close(pfd[1]);
				}
				else
				{
					close(pfd[1]);
					dup2(pfd[0], STDIN_FILENO);
					close(pfd[0]);
				}

				execve(argv[0], argv, NULL);
				
				if (errno == ENOENT)
				{
					printf("Комманда '%s' не найдена!\n", command);
				}
				
				exit(1);
			}
			else
			{
				for (i = 0; i < l_count; i++)
				{
					free(argv[i]);
				}
			}	
		}
		
		close(pfd[0]);
		close(pfd[1]);

		for ( i = 0; i < p_count; i++)
		{
			wait(NULL);
		}	
    }
    return 0;
}

