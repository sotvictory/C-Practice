#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>

/*
Пусть исполняется n процессов. Часть из них читает, а остальные пишут в разделяемую память. 
Несколько читающих процессов могут работать с буфером одновременно, но если один из процессов выполняет запись, все остальные ждут. 
Кроме того, если пишущий процесс хочет обновить запись, он должен дождаться окончания работы процессов-читателей.
(использовать семафоры для взаимного исключения).
*/

enum { n = 10 };

typedef int semaphore;

struct sembuf sops;

union semun {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
};

void lock_sem(semaphore semid)
{
    struct sembuf sops = { 0, -1, 0 };
    semop(semid, &sops, 1);
}

void unlock_sem(semaphore semid)
{
    struct sembuf sops = { 0, 1, 0 };
    semop(semid, &sops, 1);
}

void reader(int index, semaphore *mutex, semaphore *wrt, int *shared_data, int *readers_cnt)
{
    lock_sem(*mutex);
    (*readers_cnt)++;
    if (*readers_cnt == 1) // reader priority
        lock_sem(*wrt);
    unlock_sem(*mutex);
    int number = shared_data[index];
    sleep(4);
    printf("Process %d read a number %d from an array by index %d\n", getpid(), number, index);
    lock_sem(*mutex);
    (*readers_cnt)--;
    if (*readers_cnt == 0) // last reader
        unlock_sem(*wrt);
    unlock_sem(*mutex);   
}

void writer(int index, semaphore *mutex, semaphore *wrt, int *shared_data, int number)
{
    lock_sem(*wrt);
    shared_data[index] = number;
    sleep(5);
    printf("Process %d wrote a number %d into an array by index %d\n", getpid(), number, index);
    unlock_sem(*wrt);
}

int main(int argc, char **argv) 
{

    key_t key_mutex = ftok("/bin/cat", 1);
    int mutex = semget(key_mutex, 1, IPC_CREAT | 0666);
    key_t key_wrt = ftok("/bin/cat", 2);
    int wrt = semget(key_wrt, 1, IPC_CREAT | 0666);

    int shmid = shmget(key_wrt, n * sizeof(int), 0666 | IPC_CREAT);
    int *shared_data = (int *)shmat(shmid, NULL, 0);

    key_t key_readers = ftok("/bin/ls", 1);
    int readers_cnt_id = shmget(key_readers, sizeof(int), 0666 | IPC_CREAT);
    int *readers_cnt = (int *)shmat(readers_cnt_id, NULL, 0);

    if (argc == 2) {
        if (strcmp(argv[1], "--clean") == 0) {
            shmdt(shared_data);
            shmdt(readers_cnt);
            shmctl(shmid, IPC_RMID, NULL);
            shmctl(readers_cnt_id, IPC_RMID, NULL);
            semctl(mutex, 0, IPC_RMID);
            semctl(wrt, 0, IPC_RMID);
            printf("Resources have been cleared by process %d\n", getpid());
            return 0;               
        } else if (strcmp(argv[1], "--init") == 0) {
            *readers_cnt = 0;
            semctl(mutex, 0, SETVAL, 1);
            semctl(wrt, 0, SETVAL, 1);
            return 0;
        }   
    }

    char command;
    int index, number;

    while (EOF != scanf("%c %d", &command, &index)) {
        if (command == 'W') {
            scanf("%d", &number);
        }
        
        if (command == 'R') {
            reader(index, &mutex, &wrt, shared_data, readers_cnt);
        } else if (command == 'W') {
            writer(index, &mutex, &wrt, shared_data, number);
        }
    }

    return 0;
}