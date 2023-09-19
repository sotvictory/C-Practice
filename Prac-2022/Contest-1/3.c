#include <stdio.h>

int main(void)
{
    int a, b, n, i, j;

    scanf("%d %d %d", &a, &b, &n);
    
    /* шапка таблицы */
    printf(" %*d", n*2, a);
    for (i = a + 1; i < b; i++) {
        printf(" %*d", n, i);
    }
    printf("\n");

    /* все остальное */
    for (i = a; i < b; i++) {
        printf("%*d", n, i);
        for (j = a; j < b; j++) {
            printf(" %*lld", n, ((long long) i) * j);
        }
        printf("\n");
    }

    return 0;
}