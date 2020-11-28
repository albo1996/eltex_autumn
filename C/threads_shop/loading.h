#include <pthread.h>
#include <unistd.h>
#include "random_number.h"

int store[5];
pthread_mutex_t mutex[5];

void* loading(void*);

