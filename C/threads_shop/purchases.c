#include "purchases.h"

void* purchases(void* param)
{
	int j;
	int need = random_number(3000, 3500);
	
	while(1)
	{
		pthread_mutex_t mutex;
		
		pthread_mutex_init(&mutex, NULL);

		j = random_number(0, 4);

		if (store[j] == 0)
		{
			sleep(3);
			continue;
		}

		pthread_mutex_lock(&mutex);

		if (need >= store[j])
		{
			need -= store[j];
			store[j] = 0;
		}
		else
		{
			store[j] -=  need;
			need = 0;		
		}

		pthread_mutex_unlock(&mutex);
		
		pthread_mutex_destroy(&mutex);

		if(need == 0)
		{
			(*(int*)param)++;
			break;
		}
	}	
}	
