#include <signal.h>
#include <stdio.h>
#include <stdint.h>

/* это все настройка внешних сигналов 
 * обработка внутренних имеет больше нюансов */
void int_handler(int s)
{
    printf("in SIGINT\n");
}

void segv_handler(int s)
{
    printf("in SIGSEGV\n");
}

/* если скомпилировать с -std=c11, то
 * не сработает, т.к. обработка сигналов исторически
 * реализована по-разному на разных версиях UNIX */

/* handle signal */
int main()
{
    signal(SIGINT, int_handler);
    signal(SIGSEGV, segv_handler);

    int x;
    while (scanf("%d", &x) == 1) {
        if (x == 100) {
            /* зациклится и будет печатать "in SIGSEGV" 
             * segfault -- более высокоур. абстракция низкоур. прерывания pagefault 
             * после завершения прерывания процесс возвращается к инструкции,
             * которая была прервана
             * нюанс внутреннего прерывания */
            printf("%d\n", *(int *)(intptr_t) x);
        } else {
            printf("%d\n", x);
        }
    }
}