#include "basen.h"

static pthread_once_t once = PTHREAD_ONCE_INIT;
static pthread_key_t basenKey;

static void destructor(void *string)
{
	free(string);
}

static void createKey(void)
{
	pthread_key_create(&basenKey, destructor);
}

char* basen(char* pathname)
{
	char *string;
    	char *buf;
	char filename[255];
	int amount;
	int i = 0, j = 0;
	int slash = 0;

	pthread_once(&once, createKey);

	string = pthread_getspecific(basenKey);

	if (string == NULL)
	{
		string = malloc(4096);
		pthread_setspecific(basenKey, string);
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
		if (amount == 0)
		{
			strcpy(string, ".");
			break;
		}

		if (slash == 0)
		{
			break;
		}
		
	    	if (!strcmp(string, "/"))
	    	{
		    break;
	    	}

	    	if (!strcmp(string,"."))
	    	{
		    break;
	    	}
		
	    	if (!strcmp(string,".."))
	    	{
		    break;
	    	}

	    for (i = 0; slash > 0; i++)
	    {
		    if (string[i] == '/')
		    {
			    slash--;
				
			    if (slash == 0)
			    {
				    j = 0;
				    i++;
				    while (i < amount)
					{
						filename[j] = string[i];
						j++;
						i++;
					}
					
			    }
		    }
	    }
		
	strcpy(string, filename);
        break;		
    }

return string;
}




