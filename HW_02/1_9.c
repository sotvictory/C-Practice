#include <stdio.h>

void print_binary(unsigned int x) 
{
    for (int i = 31; i >= 0; i--) {
        if (x & (1 << i)) 
            printf("1");
        else 
            printf("0");
    }
}

/* invert 5, 6, 7 bits */
int main(void)
{
    unsigned int x, mask = 0b01110000; 

    scanf("%d", &x);
    print_binary(x);
    printf("\n");

    x ^= mask;
    
    print_binary(x);
    printf("\n");

    return 0;
}