#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdio.h>

void work(int rfd, int wfd)
{
    while (1) {

        int buf;

        read(rfd, &buf, sizeof(buf));
        /* начало критической секции */
        printf("%d %d\n", getpid(), buf);
        fflush(stdout); /* для файла, тк у файла полная буферизация */
        buf++;
        /* конец критической секции */
        write(wfd, &buf, sizeof(buf));

        // printf("%d %d\n", getpid(), buf);
        /* если printf поставить здесь, то вывод сломается,
         * так как процессы могут выполнить printf в любом порядке:
         * теряется синхронизация, ошибка типа race condition (гонки),
         * разделяемый ресурс -- файл, stdout
         * задача взаимного исключения
         * и printf в этом случае за пределами критической секции
         */
        
    }
}
 
int main(int argc, char **argv)
{
    int fds1[2], fds2[2];

    /* -D_GNU_SOURCE */
    pipe2(fds1, O_CLOEXEC);
    pipe2(fds2, O_CLOEXEC);

    int pid1 = fork();
    if (!pid1) {
        work(fds2[0], fds1[1]);
        _exit(1);
    }

    int pid2 = fork();
    if (!pid2) {
        work(fds1[0], fds2[1]);
        _exit(1);
    }

    /* начало работы, выход из deadlock'а */
    int buf = 1;
    write(fds2[1], &buf, sizeof(buf));

    close(fds1[1]);
    close(fds1[0]);
    close(fds2[1]);
    close(fds2[0]);

    while (wait(NULL) > 0) {}
}