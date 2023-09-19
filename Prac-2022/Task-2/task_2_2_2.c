#include <stdio.h>

int main(void) {
    double y = 0, derivative = 0, derivative_1 = 0, x, a;
    if (scanf("%lf", &x) != 1) {
        fprintf(stderr, "Error: empty input");
        return 1;
    }
    while (scanf("%lf", &a) == 1) {
        y *= x;
        y += a;
        derivative = derivative_1;
        derivative_1 *= x;
        derivative_1 += y;
    }
    printf("%.10g\n%.10g\n", y, derivative);
    return 0;
}