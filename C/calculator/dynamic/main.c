#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <dlfcn.h>
#include <sys/types.h>


int menu_size = 0;
char str[] = "./plugins/";
DIR *dir;
struct dirent *ad; 

int count_items()//Считаем количество пунктов в меню.
{
	dir = opendir(str);

	if(dir == NULL)
	{
		printf("ERROR!\n");
		exit(1);
	}

	while ((ad = readdir(dir)) != NULL)
	{
		if ((ad -> d_name[0]) == '.')
		{
			continue;
		}
	
		menu_size++;
		
	}

	closedir(dir);

	return menu_size;
}


int main()
{
	int item;
	char *fname;
	char *desc;
	char *nstr;
	void *lib_handler;

	menu_size = count_items();

	if (menu_size == 0)
		{
			printf("Папка 'plugins' пуста!\n");
			exit(1);
		}
	
	void (*menu[menu_size])();

	dir = opendir(str);

	for (int i = 0; i < menu_size; i++)
	{
		ad = readdir(dir);
	
		if ((ad -> d_name[0]) == '.')
		{
			i--;
			continue;
		};

		nstr = (char*)malloc(strlen(ad -> d_name) + strlen(str));//Создаём строку, содержащуюю полный путь до библиотеки.
		strcat(nstr, str);
		strcat(nstr, ad -> d_name);

		lib_handler = dlopen(nstr, RTLD_LAZY);

		fname = dlsym(lib_handler, "func_name");
		menu[i] = dlsym(lib_handler, fname);

		desc = dlsym(lib_handler, "description");
		printf("%d. %s\n", i + 1, desc);

		free(nstr);
		
	
	}

	printf("\n\n");

	while (1)
	{
		printf("Выберите операцию (0 - Выйти)\n\n");
		scanf("%d", &item);
		printf("\n\n");

		if (item == 0)
		{
			exit(1);
		}

		if ((item < 0) || (item > menu_size))
		{
			printf("Неккоректный пункт меню!\n\n");
		}
		else
		{
			menu[item - 1]();
		}
	}
	
	return 0;
}

