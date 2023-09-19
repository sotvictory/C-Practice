#include <stdio.h>

int main(void) {

    if ('0' < '9')
        printf("'0' < '9'\n");
    else
        printf("'0' >= '9'\n");

    if ('2' < ' ')
        printf("'2' < whitespace\n");
    else
        printf("'2' >= whitespace\n");

    if ('a' < 'B')
        printf("'a' < 'B'\n");
    else
        printf("'a' >= 'B'\n");
}