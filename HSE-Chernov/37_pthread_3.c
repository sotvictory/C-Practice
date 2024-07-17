#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>

/* -pthread 
 * ошибки можно пытаться отлавливать с помощью -fsanitize=thread 
 * (отлавливает не все) */

/* -m32 -- 32-битная компиляция */

/* ошибки data race -- ub,
 * поэтому лучше глобальные переменные не заводить */

void *thr1(void *ptr)
{
    printf("In thread %d\n", (int *)ptr);
    return NULL;
}

int main(int argc, char **argv)
{
    int count = strtol(argv[1], NULL, 10); // подаем в аргументе кол-во нитей
    pthread_t *ids = calloc(count, sizeof(ids[0]));
    int *data = calloc(count, sizeof(data[0])); // каждой нити -- свой блок

    /* размер стэка под нить фиксированного размера (getrlimit) */
    pthread_attr_t aa;
    pthread_init(&aa);
    pthread_attr_setstacksize(&aa, sysconf(_SC_THREAD_STACK_MIN));

    for (int i = 0; i < count; ++i) {
        int err = pthread_create(&ids[i], &aa, thr1, &data[i]);
        if (err) {
            fprintf(stderr, "error on thread %d: %s\n", i, strerror(err));
            exit(1);
        }
    }

    pthread_attr_destroy(&aa);

    for (int i = 0; i < count; ++i) {
        pthread_join(ids[i], NULL);
    }
}