#include <stdio.h>

int main(void) 
{
    printf("There are %ld bytes in short\n", sizeof(short int));
    printf("There are %ld bytes in int\n", sizeof(int));
    printf("There are %ld bytes in long\n", sizeof(long));
    printf("There are %ld bytes in long long\n", sizeof(long long));    
    printf("There are %ld bytes in float\n", sizeof(float));
    printf("There are %ld bytes in double\n", sizeof(double));
    printf("There are %ld bytes in long double\n", sizeof(long double));

    return 0;
}