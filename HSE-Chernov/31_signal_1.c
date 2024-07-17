#include <signal.h>
#include <stdio.h>
#include <stdint.h>

/* ignore signal */
int main()
{
    /* если отправить kill'ом (др. процессом) -- игнорируется
     * самой программой -- нет (ядро):
     * внутренняя ошибка => обработка по умолчанию */
    signal(SIGSEGV, SIG_IGN);

    int x;
    while (scanf("%d", &x) == 1) {
        /* недопустимое обращение к памяти */
        printf("%d\n", *(int *)(intptr_t) x);
    }
}