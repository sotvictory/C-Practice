#include <stdio.h>

int main(void)
{
    int a, b;

    scanf("%d%d", &a, &b);
    printf("%lld\n", (long long) a + b);

    return 0;
}