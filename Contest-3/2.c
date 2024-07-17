#include <stdio.h>
#include <stdint.h>

int main(void)
{
    int N;
    scanf("%d", &N);

    uint64_t X[N];
    for (int i = 0; i < N; i++)
        scanf("%lx", &X[i]);

    uint64_t S;
    scanf("%lx", &S);

    int i_min = -1, j_min = -1;
    for (int i = 0; i < N; i++) {
        for (int j = i + 1; j < N; j++) {
            if (X[i] + X[j] == S) {
                if (i_min == -1 || (X[i] < X[i_min] || (X[i] == X[i_min] && X[j] < X[j_min]))) {
                    i_min = i;
                    j_min = j;
                }
            }
        }
    }

    if (i_min == -1) {
        printf("-1 -1\n");
    } else {
        printf("%d %d\n", i_min, j_min);
    }

    return 0;
}