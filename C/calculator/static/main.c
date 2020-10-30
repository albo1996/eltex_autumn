#include <stdlib.h>
#include <stdio.h>
#include "calc_lib.h"

int main()
{
	int menu_item;
	double a, b;

	printf("Калькулятор\n\n");

	printf(	"Доступные действия:\n\n"
			"1. Сложение\n"
			"2. Вычитание\n"
			"3. Умножение\n"
			"4. Деление\n"
			"0. Выйти\n\n"
			);

	while(1)
	{
		printf("\n");

		printf("Какое действие выполнить?\n");
		scanf("%d", &menu_item);

		printf("\n");

		switch (menu_item)
		{

		case 1:
			printf("Введите первое число: ");
			scanf("%lf", &a);

			printf("Введите второе число: ");
			scanf("%lf", &b);

			printf("\n");

			printf("%.4lf\n\n", add(a, b));
			break;
		case 2:
			printf("Введите первое число: ");
			scanf("%lf", &a);

			printf("Введите второе число: ");
			scanf("%lf", &b);

			printf("\n");

			printf("%.4lf\n\n", sub(a, b));
			break;
		case 3:
			printf("Введите первое число: ");
			scanf("%lf", &a);

			printf("Введите второе число: ");
			scanf("%lf", &b);

			printf("\n");

			printf("%.4lf\n\n", mul(a, b));
			break;
		case 4:
			printf("Введите первое число: ");
			scanf("%lf", &a);

			printf("Введите второе число: ");
			scanf("%lf", &b);

			printf("\n");

			printf("%.4lf\n\n", divi(a, b));
			break;
		case 0:
			exit(0);

		default:
			printf("Некорректный пункт меню!\n\n");
			break;
		}
	}
	
	return 0;
}

