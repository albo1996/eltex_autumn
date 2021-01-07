#include "lexemes_count.h"

int lexemes_count(char* s)
{
	char* ptr;
	char buf[50];
	int count = 0;

	strncpy(buf, s, strlen(s));

	ptr = strtok(buf, " ");

	while (ptr != NULL)
	{
		count++;
		ptr = strtok(NULL, " ");
	}
	
	return count;
}