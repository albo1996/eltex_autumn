#include <stdio.h>

char func_name[] = "add";
char description[] = "Addition";

void add()
{	double first_number, second_number, result;

	printf("Enter the first number:\n");
	scanf("%lf", &first_number);
	
	printf("Enter the second number:\n");
	scanf("%lf", &second_number);
	
	result = first_number + second_number;
	
	printf("Result: %.2lf\n\n", result);
	

}
