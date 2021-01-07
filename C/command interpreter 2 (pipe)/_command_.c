#include "_command_.h"


void _command_ (char* s, char** a, int count)
{
	char *ptr;
	int i;

	ptr = strtok(s, "|");

	for ( i = 0; i < count; i++)
	{
		a[i] = strdup(ptr);
		ptr = strtok(NULL, "|");
	}
}
