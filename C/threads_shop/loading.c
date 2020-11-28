#include "loading.h"

void* loading(void* param)
{
	int j;
	
	while(*(int*)param < 3)
	{
		j = random_number(0, 4);

		store[j] += random_number(300, 500);

		sleep(1);
	}
}

