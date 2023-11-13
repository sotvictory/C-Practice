#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <stdint.h>

/* -pthread 
 * ошибки можно пытаться отлавливать с помощью -fsanitize=thread 
 * (отлавливает не все) */

/* ошибки data race -- ub */

void *thr1(void *ptr)
{
    printf("In thread %d\n", (int)(intptr_t)ptr);
    return NULL;
}

int main(int argc, char **argv)
{
    int count = strtol(argv[1], NULL, 10); // подаем в аргументе кол-во нитей
    pthread_t *ids = calloc(count, sizeof(ids[0]));

    for (int i = 0; i < count; ++i) {
        int err = pthread_create(&ids[i], NULL, thr1, (void *)(intptr_t)i);
        if (err) {
            fprintf(stderr, "error: %s\n", strerror(err));
            exit(1);
        }
    }

    for (int i = 0; i < count; ++i) {
        pthread_join(ids[i], NULL);
    }
}