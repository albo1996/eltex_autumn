#include "purchases.h"
#include "loading.h"
#include "random_number.h"
#include <time.h>


int i;
int sum = 0;//Количество завершившихся потоков

int main()
{
	pthread_t loader;
	pthread_t customers[3];

	srand(time(NULL));
	
	for(i = 0; i < 5; i++)
	{
		store[i] = random_number(1000, 1200);
		pthread_mutex_init(&mutex[i], NULL);
	}

	pthread_create(&loader, NULL, loading, &sum);

	for(int i = 0; i < 3; i++)
	{
    	pthread_create(&customers[i], NULL, purchases, &sum);
	}
	
	pthread_join(loader, NULL);
	pthread_join(customers[0], NULL);
	pthread_join(customers[1], NULL);
	pthread_join(customers[2], NULL);

	printf("\n");
	printf("All needs of customers are met!\n\n");

	printf("Leftovers:\n");

	for ( i = 0; i < 5; i++)
	{
		printf("STORE[%d] = %d\n", i, store[i]);
	}
	
	return 0;
}
