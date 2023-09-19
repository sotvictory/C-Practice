#include <stdio.h>
#include <math.h>

double sum_sin(int n, double x)
{
    double sum = 0;
    int i;

    for (i = 0; i < n; i++) {
        sum += sin(x);
        x = sin(x);
    }

    return sum;
}

int main(void)
{
    double x = 3.14;
    int n = 2;

    printf("The sum of sines for n = %d and x = %f is equal to %f\n", n, x, sum_sin(n, x));

    return 0;
}