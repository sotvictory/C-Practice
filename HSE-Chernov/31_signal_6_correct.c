#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

volatile sig_atomic_t flag = 0;

void int_handler(int s)
{
    flag = 1;
}

/* решение без race condition */
int main(void)
{
    /* block sigint */
    sigset_t ss, os;
    sigemptyset(&ss);
    sigaddset(&ss, SIGINT);
    sigempty(&os);
    sigprocmask(SIG_BLOCK, &ss, NULL);

    sigaction(SIGINT, &(struct sigaction) {.sa_flags = SA_RESTART,\ 
                .sa_handler = int_handler}, NULL);

    while (1) {
        if (!flag) {
            #if 0
            /* так работает sigsuspend() */
            sigprocmask(SIG_SETMASK, &os, &ts);
            pause();
            sigprocmask(SIG_SETMASK, &ts);
            /* три этих функции внутри sigsuspend() реализованы атомарно */
            #endif

            /* однако концептуально теперь работа с сигналами синхронна (это не всегда хорошо)
             * и sigsuspend() ничем не отличается от read() */
            sigsuspend(&os);

            #if 0
            sigprocmask(SIG_UNBLOCK, &ss), NULL);
            /* здесь race condition, надо заменить на sigsuspend() */
            pause();
            sigprocmask(SIG_BLOCK, &ss), NULL);
            #endif
        }
        flag = 0;
        printf("SIGINT\n");
    }
}