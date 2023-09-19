#include <stdio.h>

int main(void) {
    double y = 0, x, a;
    if (scanf("%lf", &x) != 1) {
        fprintf(stderr, "Error: empty input");
        return 1;
    }
    while (scanf("%lf", &a) == 1) {
        y *= x;
        y += a;
    }
    printf("%.10g\n", y);  
    return 0;
}