#include <stdio.h>
#include <limits.h>
#include <stdlib.h>

int main(int argc, char **argv) 
{
    long long sum = 0, num= 0;
    unsigned long long new_sum = 0;
    int i;

    for (i = 1; i < argc; i++) {
        num = atoll(argv[i]);
        if (sum > LLONG_MAX - num) {
            new_sum = sum;
            new_sum += num;
            printf("Intermediate sum exceeds long long, using unsigned long long instead\n");
            printf("New sum: %llu\n", new_sum);
            sum = new_sum;
        } else {
            sum += num;
        }
    }

    printf("Sum: %lld\n", sum);

    return 0;
}