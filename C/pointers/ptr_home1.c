#include "stdio.h"


int main()
{
    int a = 14031996;
    char* ptr;

    ptr = &a;

    printf("1:%d\n", *ptr);//Первый байт
    ptr++;

    printf("2:%d\n", *ptr);//Второй байт. Выводим и меняем его значение.
    *ptr=12;
    ptr++;

    printf("3:%d\n", *ptr);//Третий байт
    ptr++;

    printf("4:%d\n", *ptr);//Четвёртый байт

    printf("a = %d\n", a);//Выводим новое значение a

}
