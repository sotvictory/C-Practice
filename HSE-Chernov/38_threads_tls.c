#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

/* -pthread */

/* у каждой нити своя переменная,
 * но все они находятся в общем адресном пространстве */
_Thread_local int var = 100;

void *func(void *ptr)
{
    printf("%d\n", ++var);
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