#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>


int main()
{
    pid_t pid;
    int i;
    extern int errno;
    int status;
    int cmp;
    char *argv[2];
    char path[]="/bin/";
    char command[50];
    
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
    	
    	argv[0] = (char*)malloc(strlen(path) + strlen(command) + 1);
    	strcpy(argv[0], path);
    	strncat(argv[0], command, strlen(command));
    	
    	pid = fork();
    	
    	if (pid == 0)
    	{
    		execv(argv[0], argv);
    		
    		if (errno == ENOENT)
    		{
    			printf("Комманда '%s' не найдена!\n", command);
    		}
    		
    		exit(1);
    	}
    	else
    	{
    		waitpid(pid, &status, __WALL);
    		free(argv[0]);
    	}
    }
    return 0;
}
