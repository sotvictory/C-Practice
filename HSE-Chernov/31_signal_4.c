#include <signal.h>
#include <stdio.h>
#include <stdint.h>
#include <errno.h>
#include <string.h>

void int_handler(int s)
{
    /* надо делать так: */
    int e = errno;
    write(1, "in SIGINT\n", 10);
    errno = e;
    /* errno -- глобальная, обработчик может ее испортить */
}

void segv_handler(int s)
{
    /* printf не асинхронно-безопасный,
     * так делать не надо */
    printf("in SIGSEGV\n");
}

/* корректная обработка и в BSD, и в system V: sigaction()
 * POSIX совместимость компенсирует неудобства испол ьзования
 * -std=gnu11
 * -std=c11 -D_POSIX_C_SOURCE=19990101 */
int main()
{
    signal(SIGINT, int_handler);
    signal(SIGSEGV, segv_handler);

    sigaction(SIGINT, &(struct sigaction) {.sa_flags = SA_RESTART, .sa_handler = int_handler}, NULL);

    int x;
    /* костыль 1: обработка сигнала */
    while (scanf("%d", &x) == 1) {
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