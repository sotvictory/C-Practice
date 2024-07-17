#include <stdio.h>

double myAbs(double x) {
    if (x < 0)
        return -x;
    else
        return x;
}

double mySqrt(double x, double e) {
    double x_i = 1, x_i_1;
    int i = 0;
    x_i_1 = (x_i + x / x_i) / 2;
    while (myAbs(x_i - x_i_1) > e) {
        x_i = x_i_1;
        x_i_1 = (x_i + x / x_i) / 2;
    }
    return x_i_1;
}

int main(void) {
    double x, e;
    scanf("%lf", &e);
    while (scanf("%lf", &x) == 1) {
        if (x < 0) {
            fprintf(stderr, "Error: attempt to calculate the root of a negative number");
            return 1;
        } else {
            printf("%.10g\n", mySqrt(x, e));
        }
    }
    return 0;
}