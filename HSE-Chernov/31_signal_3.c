#include <signal.h>
#include <stdio.h>
#include <stdint.h>
#include <errno.h>
#include <string.h>

void int_handler(int s)
{
    /* костыль 2: переустановка обработчика */
    signal(SIGINT, int_handler);
    printf("in SIGINT\n");
}

void segv_handler(int s)
{
    printf("in SIGSEGV\n");
}

/* -std=c11 -- режим system V совместимости
 * -std=gnu11 -- режим BSD совместимости
 * корректная обработка и в BSD, и в system V:
 * исторически обработка сигналов отличается */
int main()
{
    signal(SIGINT, int_handler);
    signal(SIGSEGV, segv_handler);

    int x;
    /* костыль 1: обработка сигнала */
    while (1) {
        int r;
        do {
            r = scanf("%d", &x);
        } while (r == -1 && errno == EINTR);
        if (x == 100) {
            printf("%d\n", *(int *)(intptr_t) x);
        } else {
            printf("%d\n", x);
        }
    }

    if (ferror(stdin)) {
        fprintf(stderr, "error: %s\n", strerror(errno));
    }
}