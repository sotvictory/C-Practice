#include <stdio.h>

int main(void)
{
    int sum = 0, c;

    while (((c = getchar()) != EOF)) {
        if ((c >= '0') && (c <= '9')) {
            sum += c - '0';
        } else if ((c >= 'a') && (c <= 'f')) {
            sum += c - 'a' + 10;
        } else if ((c >= 'A')&&(c <= 'F')) {
            sum += c - 'A' + 10;
        }
    }
    printf("%d\n", sum);

    return 0;
} 
