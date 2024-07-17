/* 
 * -+-+CBDCA
 * ((+-+CBDC) - A)
 * ((-+CBD) + C) - A)
 * (((+CB) - D) + C) - A)
 * (((C + B) - D) + C) - A)
*/

#include <stdio.h>

int main(void)
{
    long long int A, B, C, D, res;

    scanf("%lld", &A);
    scanf("%lld", &B);
    scanf("%lld", &C);
    scanf("%lld", &D);

    res = C + B - D + C - A;

    printf("%lld\n", res);

    return 0;
}