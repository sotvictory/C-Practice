#include <stdio.h>

void hocus_pocus(int *a, int *b)
{
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

int holy_function(int a, int b)
{
    int result = a * b;

    while (b != 0) {
        hocus_pocus(&a, &b);
        b = b % a;
    }

    return result / a;
}

int main(void)
{
    int a, b;

    scanf("%d %d", &a, &b);
    printf("%d\n", holy_function(a, b));

    return 0;
}