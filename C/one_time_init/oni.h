#include <stdbool.h>
#include <pthread.h>


typedef struct
{
    bool indicator;
    pthread_mutex_t mutex;
} once_control;

int one_time_init(once_control*, void*(void));
