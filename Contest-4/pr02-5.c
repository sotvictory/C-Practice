#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <math.h>

int last_prime = 0;

void int_handler(int s)
{
    static int cnt = 0;

    cnt++;

    if (cnt == 4) {
        exit(0);
    }

    printf("%d\n", last_prime);
    fflush(stdout);
}

void term_handler(int s)
{
    exit(0);
}

int main(void)
{
    int low, high, i, j, root, is_prime = 0, is_even = 0;

    scanf("%d %d", &low, &high);
    
    printf("%d\n", getpid());
    fflush(stdout);

    signal(SIGINT, &int_handler);
    signal(SIGTERM, &term_handler);

    /* a negative number or 0 or 1 is not prime,
     * iterating over values starts from 2 */
    if (low <= 1) {
        low = 2;
    } else if (low == 2) {
        low = 3;
        last_prime = 2;
    }

    if (low % 2 == 0) {
        is_even = 1;
    }

    /* odd numbers are not prime */
    for (i = low + is_even; i < high; i += 2) {
        is_prime = 1;
        root = sqrt(i) + 1;
        for (j = 3; j <= root; j += 2) {
            if (i % j == 0) {
                is_prime = 0;
                break;
            }
        }
        if (is_prime == 1) {
            last_prime = i;
        }
    }

    printf("-1\n");
    fflush(stdout);
    exit(0);
}