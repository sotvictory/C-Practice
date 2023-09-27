#include <stdio.h>
#include <limits.h>

void char_representation_1(void)
{
    if (CHAR_MIN == 0)
        printf("Char is unsigned by default\n");
    else
        printf("Char is signed by default\n");
}

void char_representation_2(void)
{
    char c = 126;

    c += 2;

    if (c > 126)
        printf("Char is unsigned by default\n");
    else
        printf("Char is signed by default\n");
}

int main(void) 
{
    char_representation_1();
    char_representation_2();

    return 0;
}