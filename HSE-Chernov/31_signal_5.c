#include <signal.h>
#include <stdlib.h>
#include <stdio.h>

/* запрещает компилятору предполагать, что
 * значение переменной не может измениться, если
 * компилятор этого не видит
 * в частности запрещает размещение этой переменной на регистре */
volatile int flag = 0;

/* можно использовать гарантированно атомарный для совместимости со "странным" оборудованием: */
volatile sig_atomic_t flag_1 = 0;

void int_handler(int s)
{
    flag = 1;
}

/* если включить оптимизацию -02, то программа сломается 
 * перестанет реагировать на сигналы 
 * код, сгенерированный компилятором:
 * gcc 31_signal_5.c -02 -S -fverbose-asm */
int main()
{
    sigaction(SIGINT, &(struct sigaction) {.sa_flags = SA_RESTART, .sa_handler = int_handler}, NULL);

    while (1) {
        while (!flag) {}
        flag = 0;
        printf("SIGINT\n");
    }

    #if 0
    /* компилятором заменено на: */
    while (1) {
        /* это плохо, потому что программа находится в состоянии R+
         * и потребляет процессорное время (посмотреть htop) и энергию
         * и мы здесь не делаем ничего полезного, мы лишь ждем
         * наступление события -- активное ожидание (busy wait) */
        if (!flag) {
            while (!flag) {}
        }
        flag = 0;
        printf("SIGINT\n");
    }
    /* так значения flag во втором while не модифицируется 
     * то есть компилятор не учел, что flag может изменяться там,
     * где он это не отслеживает
     * если программа может исполняться асинхронно / в другом потоке исполнения,
     * то об этом нужно сказать явно: ключевое слово volatile */ 
    #endif
}