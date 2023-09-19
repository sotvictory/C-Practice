#include <stdio.h>

int main(void) {
    int n;
    double y = 0, derivative = 0, previous_derivative = 0, integral = 0, x, a ;
    if (scanf("%d%lf", &n, &x) != 2) {
        fprintf(stderr, "Error: incorrect input");
        return 1;
    }
    while (scanf("%lf", &a) == 1) {
        y *= x;
        y += a;
        derivative = previous_derivative;
        previous_derivative *= x;
        previous_derivative += y;
        integral *= x;
        integral += a / (n-- + 1);
    }
    integral *= x;
    printf("%.10g\n%.10g\n%.10g\n", y, derivative, integral);
    return 0;
}