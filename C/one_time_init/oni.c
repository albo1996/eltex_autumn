#include "oni.h"


int one_time_init(once_control* c, void *(init)(void))
{
	pthread_mutex_lock(&c -> mutex);
	
	if (c -> indicator)
	{
		return 1;
   	}
   	else
   	{
        c -> indicator = true;
        	
        init();
	}
	
	pthread_mutex_unlock(&c -> mutex);
    
    return 0;
}
