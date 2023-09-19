#include <stdio.h>
#include <time.h>

int iterativeFibonacci(int i) {
    int f_0 = 0, f_1 = 1, j, f_i;
    if (i == 0)
        return 0;
    else if (i == 1)
        return 1;
    for (j = 2; j <= i; j++) {
        f_i = f_0 + f_1;
        f_0 = f_1;
        f_1 = f_i;
    }
    return f_i;
}

int recursiveFibonacci(int i) {
    if (i == 0)
        return 0;
    else if (i == 1)
        return 1;
    else 
        return recursiveFibonacci(i - 2) + recursiveFibonacci(i - 1);
}

int main(void) {
    int i;
    double processTime;
    clock_t beginTime, endTime;
    while (scanf("%d", &i) == 1) {
        beginTime = clock();
        printf("%d\n", iterativeFibonacci(i));
        endTime = clock();
        processTime = endTime - beginTime;
        fprintf(stderr, "Calculations took %.10g clocks per second\n", processTime);
        beginTime = clock();
        printf("%d\n", recursiveFibonacci(i));
        endTime = clock();
        processTime = endTime - beginTime;
        fprintf(stderr, "Calculations took %.10g clocks per second\n", processTime);
        printf("\n");
    }
    return 0;
}