#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

/* -pthread */

pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
_Thread_local int var = 100;

void *func(void *ptr)
{
    pthread_mutex_lock(&mutex);
    printf("%d\n", ++var);
    pthread_mutex_unlock(&mutex);
    return NULL;
}

int main(void)
{
    pthread_t t1, t2;

    pthread_create(&t1, NULL, func, NULL);
    pthread_create(&t2, NULL, func, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
}