#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

volatile sig_atomic_t flag = 0;

void int_handler(int s)
{
    flag = 1;
}

int main()
{
    sigaction(SIGINT, &(struct sigaction) {.sa_flags = SA_RESTART, .sa_handler = int_handler}, NULL);

    while (1) {
        /* убираем активное ожидание: R+ -> S+ */
        if (!flag) {
            /* передаем управление ядру ОС 
             * оно разбудит, когда произойдет соответствующее событие */
            
            /* однако здесь race condition:
             * если сигнал пришел, flag установлен в 1
             * при этом проверку мы уже выполнили */
            pause();
        }
        flag = 0;
        printf("SIGINT\n");
    }
}