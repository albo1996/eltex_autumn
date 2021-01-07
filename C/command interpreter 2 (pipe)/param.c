#include "param.h"


void param(char** a, char* s, char* path, int count)
{
	int i;
	char* ptr;

	ptr = strtok(s, " ");

	for ( i = 0; i < count; i++)
	{
		if (i == 0)
		{
			a[i] = (char*)malloc(strlen(path) + strlen(ptr) + 1);
			strcpy(a[i], path);
			strncat(a[i], ptr, strlen(ptr));
			ptr = strtok(NULL, " ");
		}
		else
		{
			a[i] = strdup(ptr);
			ptr = strtok(NULL, " ");
		}
	}

	a[count] = NULL;
}
