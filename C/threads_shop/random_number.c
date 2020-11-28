#include "random_number.h"

int random_number(int a, int b)
{
	int num;

	num = a + rand() % (b - a + 1);

	return num;
}
