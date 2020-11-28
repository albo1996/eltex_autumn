#include "loading.h"

void* loading(void* param)
{
	int j;
	
	while(*(int*)param < 3)
	{
		j = random_number(0, 4);

		pthread_mutex_lock(&mutex[j]);

		store[j] += random_number(300, 500);

		pthread_mutex_unlock(&mutex[j]);

		sleep(1);
	}
}

