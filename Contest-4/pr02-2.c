#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

int main(void)
{
    int N, i;

    scanf("%d", &N);

    for (i = 1; i <= N; i++) {
        if (fork() == 0) {
            if (i != N) {
                printf("%d ", i);
                fflush(stdout);
            } else {
                printf("%d\n", i);
                fflush(stdout);
            }
            exit(0);
        }
        wait(NULL);
    }
    
    exit(0);
}