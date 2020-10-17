#include "stdio.h"


int main()
{   
    char a = 'A';
    char b = 'B';
    char* ptr1;
    char** ptr2;

    ptr1=&a;

    ptr2=&ptr1;
    
    *ptr2=&b;

    printf("*ptr1 -> %c\n", *ptr1);
    
    return 0;
    
}
