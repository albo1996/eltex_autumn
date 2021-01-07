#include "command_count.h"


int command_count(char* s)
{
	int count = 0;
	int i;
	char buf[512];
	char* ptr;

	strncpy(buf, s, strlen(s));

	ptr = strtok(buf, "|");

	while (ptr != NULL)
	{
		count++;
		ptr = strtok(NULL, "|");
	}
	
	return count;
}