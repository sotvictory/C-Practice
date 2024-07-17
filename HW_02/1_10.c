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

/* set all bits to 0 (except the first 5)
 * set first 3 bits to 1 */
int main(void)
{
    unsigned int z, mask_1 = 0b00011111, mask_2 = 0b00000111; 

    scanf("%d", &z);
    print_binary(z);
    printf("\n");

    z &= mask_1;
    z |= mask_2;
    
    print_binary(z);
    printf("\n");

    return 0;
}