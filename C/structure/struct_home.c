#include "stdio.h"
#include "stdlib.h"
#include "string.h"


int amt = 0;//Количество записей

struct phonebook
{
    int id;
    char name[20];
    char surname[20];
    char number[15];
};

struct phonebook* subs;


void print_menu()//Вывод меню
    {      
        printf("Выберите пункт меню:\n\n"
			"1. Добавить запись\n"
			"2. Удалить запись\n"
			"3. Поиск записи по номеру телефона\n"
			"4. Показать все записи\n"
			"0. Выйти\n\n"
			);
    }

void add_sub()//1
{
    
    if (amt == 0)
    {
       subs = (struct phonebook*) malloc(sizeof(struct phonebook));
    } 

    if (amt > 0)
    {
        subs = (struct phonebook*) realloc(subs, (amt + 1) * sizeof(struct phonebook));
    }
	

    printf("Введите имя абонента: \n");
    scanf("%s", subs[amt].name);
    

    printf("Введите фамилию абонента: \n");
    scanf("%s", subs[amt].surname);
  

    printf("Введите номер телефона абонента: \n");
    scanf("%s", subs[amt].number);
    
    

    printf("\n");
    printf("Запись внесена\n\n");

    subs[amt].id = amt;

    amt++;

}

void remove_sub()//2
{
    if (amt == 0)
    {
        printf("В справочнике нет записей!\n\n");

    }
    
    if (amt > 0)
    {
        int id_r;//Номер удаляемой записи
        int i;
        printf("Введите номер записи, которую нужно удалить: \n");
        scanf("%d", &id_r);

        if (id_r > amt - 1)
        {
            printf("Нет записи с таким номером!\n\n");
        }

        else
        {
            struct phonebook* new_subs = (struct phonebook*) malloc((amt - 1) * sizeof(struct phonebook));

            for (i = 0; i < amt - 1; i++)
            {
                if (i >= id_r)
                {
                    new_subs[i].id = i; 
                    strcpy(new_subs[i].name, subs[i+1].name);
                    strcpy(new_subs[i].surname, subs[i+1].surname);
                    strcpy(new_subs[i].number, subs[i+1].number);
                }

                if (i < id_r)
                {
                    new_subs[i].id = i; 
                    strcpy(new_subs[i].name, subs[i].name);
                    strcpy(new_subs[i].surname, subs[i].surname);
                    strcpy(new_subs[i].number, subs[i+1].number);

                }
            }

            free(subs);
            subs = new_subs;
            new_subs = NULL;

            amt--;

            printf("\n");
            printf("Запись удалена\n\n");
        }
    }
}

void sub_search()//3
{  
    int i;
    char sought_number[15];

    if (amt == 0)
    {
       printf("В справочнике нет записей!\n\n"); 
    }

    if (amt > 0)
    {
        printf("Введите номер телефона абонента:\n\n");
        scanf("%s", sought_number);
        
        printf("\n\n");
       

        for (i=0; i < amt; i++)
        {
            if (strcmp(subs[i].number, sought_number) == 0)
            {
                printf("Фамилия: %s\n"
                        "Имя: %s\n"
                        "Номер абонента: %s\n\n", subs[i].surname, subs[i].name, subs[i].number);
                          
            }

        }
    }
    
}

void show_phonebook()//4
{ 
    int i;
    if (amt == 0)
    {
        printf("В справочнике нет записей!\n\n");
    }

    if (amt > 0)
    {
    
    printf("Все записи:\n\n");
    
        for (i = 0; i < amt; i++) 
        {
	        printf("ID: %d\n"
			    "Фамилия: %s\n"
			    "Имя: %s\n"
			    "Номер абонента: %s\n\n", subs[i].id, subs[i].surname,
			    subs[i].name, subs[i].number);
	    }
    }
}


int main()
{   
    int item;

    printf("Абонентский справочник \n\n");
    print_menu();
   
   while (1)
   {
        printf("> ");
        scanf("%d", &item);//Ввод пункта меню
        printf("\n");

        switch(item)
        {
            case 1:
                add_sub();
                break;
            case 2:
                remove_sub();
                break;
            case 3:
                sub_search();
                break;
            case 4:
                show_phonebook();
                break;
            case 0:
                exit(0);
            default:
                printf("Некорректный пункт меню!\n\n");

        } 
   } 

return 0;

}
