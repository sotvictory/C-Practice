#include <stdio.h>
#include <limits.h>
#include <stdlib.h>

int main(int argc, char *argv[]) 
{
    if (argc < 2) 
    {
        fprintf(stderr, "Usage: %s <number1> <number2> ...\n", argv[0]);
        return 1;
    }

    long long sum = 0;
    for (int i = 1; i < argc; i++) 
    {
        long long num = atoll(argv[i]);
        if (sum > LLONG_MAX - num)
        {
            unsigned long long new_sum = sum;
            new_sum += num;
            printf("Intermediate sum exceeds long long, using unsigned long long instead\n");
            printf("New sum: %llu\n", new_sum);
            sum = new_sum;
        } 
        else 
        {
            sum += num;
        }
    }

    printf("Sum: %lld\n", sum);

    return 0;
}