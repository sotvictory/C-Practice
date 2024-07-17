#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

/*
Пусть исполняется n процессов. Часть из них читает, а остальные пишут в разделяемую память. 
Несколько читающих процессов могут работать с буфером одновременно, но если один из процессов выполняет запись, все остальные ждут. 
Кроме того, если пишущий процесс хочет обновить запись, он должен дождаться окончания работы процессов-читателей.
(использовать семафоры для взаимного исключения).
*/

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
    printf("Process %d wrote a number %d into an array by index %d\n", getpid(), number, index);
    unlock_sem(*wrt);
}

int main(int argc, char **argv) 
{
    if (argc != 2) {
        printf("Usage: %s n\n", argv[0]);
        return 1;
    }

    int n = atoi(argv[1]);
    pid_t pid[n];
    int fds[n][2];

    key_t key_mutex = ftok("/bin/cat", 1);
    int mutex = semget(key_mutex, 1, IPC_CREAT | 0666);
    key_t key_wrt = ftok("/bin/cat", 2);
    int wrt = semget(key_wrt, 1, IPC_CREAT | 0666);

    semctl(mutex, 0, SETVAL, 1);
    semctl(wrt, 0, SETVAL, 1);

    int shmid = shmget(key_wrt, n * sizeof(int), 0666 | IPC_CREAT);
    int *shared_data = (int *) shmat(shmid, NULL, 0);

    key_t key_readers = ftok("/bin/ls", 1);
    int readers_cnt_id = shmget(key_readers, sizeof(int), 0666 | IPC_CREAT);
    int *readers_cnt = (int *) shmat(readers_cnt_id, NULL, 0);
    *readers_cnt = 0;

    for (int i = 0; i < n; i++) {
        pipe(fds[i]);
        if ((pid[i] = fork()) == 0) {
            close(fds[i][1]);
            while (1) {
                char command;
                int index;
                read(fds[i][0], &command, sizeof(command));
                read(fds[i][0], &index, sizeof(index));
                if (command == 'R') {
                    printf("READER started\n");
                    reader(index, &mutex, &wrt, shared_data, readers_cnt);
                    printf("READER ended\n");
                } else if (command == 'W') {
                    printf("WRITER started\n");
                    int number;
                    read(fds[i][0], &number, sizeof(number));
                    writer(index, &mutex, &wrt, shared_data, number);
                    printf("WRITER ended\n");
                }
            }
            close(fds[i][0]);
        }
    }

    char command;
    int process_num, index, number;

    while (3 == scanf("%c %d %d", &command, &process_num, &index)) {
        printf("PARENT started reading\n");
        write(fds[process_num - 1][1], &command, sizeof(command));
        write(fds[process_num - 1][1], &index, sizeof(index));
        if (command == 'W') {
            scanf("%d", &number);
            write(fds[process_num - 1][1], &number, sizeof(number));
        }
        printf("PARENT finished reading\n");
    }
    printf("End of scanf\n");

    for (int i = 0; i < n; i++) {
        wait(NULL);
    }

    shmdt(shared_data);
    shmdt(readers_cnt);
    shmctl(shmid, IPC_RMID, NULL);
    shmctl(readers_cnt_id, IPC_RMID, NULL);
    semctl(mutex, 0, IPC_RMID);
    semctl(wrt, 0, IPC_RMID);

    return 0;
}