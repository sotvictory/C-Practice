#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

/* -pthread */

void *thr1(void *ptr)
{
    printf("In thread\n");
    /* exit в любой нити завершает процесс целиком */
    //exit(0);
    return NULL;
}

int main(int argc, char **argv)
{
    pthread_t id = 0;

    int err = pthread_create(&id, NULL, thr1, NULL);
    if (err) {
        fprintf(stderr, "error: %s\n", strerror(err));
    }
    /* порядок печати так же не гарантируется */
    printf("In main\n");
    //pthread_join(id, NULL);
    /* завершить главную нить как нить, а не как процесс */
    pthread_exit(NULL);
}