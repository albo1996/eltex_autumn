#include <pthread.h>
#include <unistd.h>
#include "random_number.h"

int store[5];

void* loading(void*);

