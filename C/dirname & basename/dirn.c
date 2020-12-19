#include "dirn.h"

static pthread_once_t once = PTHREAD_ONCE_INIT;
static pthread_key_t dirnKey;

static void destructor(void *string)
{
	free(string);
}

static void createKey(void)
{
	pthread_key_create(&dirnKey, destructor);
}

char* dirn(char* pathname)
{
	char *string;
    char *buf;
	int amount;
	int i = 0, j = 0;
	int slash = 0;

	pthread_once(&once, createKey);

	string = pthread_getspecific(dirnKey);

	if (string == NULL)
	{
		string = malloc(4096);
		pthread_setspecific(dirnKey, string);
	}
	
    strcpy(string, pathname);

    amount = strlen(string);

    buf = (char*)malloc(amount + 1);

	while (i < amount)
	{
		if ((string[i] == '/') && (string[i - 1] == '/'))
		{
			i++;
		}
		else
		{
			buf[j] = string[i];
			i++;
			j++;
		}
	}

	strcpy(string, buf);
    free(buf);

    amount = strlen(string);

    if ((string[amount - 1] == '/') && (amount != 1))
    {
	    string[amount - 1] = 0;
	    amount = strlen(string);
    }

	for (i = 0; i < amount; i++)
	{
		if (string[i] == '/')
	    {
			slash++;
	    }
	}
	
	if ((string[amount - 1] == '/') && (strcmp(string, "/") == 1))
    {
	    slash--;
	    string[amount - 1] = 0;
	    amount--;
    }

    while (1)
    {
	    if ((!strcmp(string, "/")) || (slash == 1) && (string[0] == '/'))
	    {
		    strcpy(string,"/");
		    break;
	    }

	    if ((amount = 0) || (slash == 0) || (amount > 0) && (slash == 1))
	    {
		    strcpy(string,".");
		    break;
	    }
		
	    if (!strcmp(string,".."))
	    {
	    	strcpy(string,".");
		    break;
	    }

	    for (i = 0; slash > 0; i++)
	    {
		    if (string[i] == '/')
		    {
			    slash--;
				
			    if (slash == 0)
			    {
				    string[i] = 0;
			    }
		    }
	    }
        
		break;		
    }

	return string;
}




