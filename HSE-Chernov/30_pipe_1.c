#include <unistd.h>
#include <sys/wait.h>

/* ls | cat
 * classic pipe() */
int main(int argc, char **argv)
{
    int fds[2];

    pipe(fds);

    int pid1 = fork();
    if (!pid1) {
        dup2(fds[1], 1);
        close(fds[0]);
        close(fds[1]);
        execlp(argv[1], argv[1], NULL);
        _exit(1);
    }

    close(fds[1]);

    int pid2 = fork();
    if (!pid2) {
        dup2(fds[0], 0);
        close(fds[0]);
        execlp(argv[2], argv[2], NULL);
        _exit(1);
    }

    close(fds[0]);

    /* ждет всех своих сыновей
     * wait возвращает pid1 или pid2 */
    while (wait(NULL) > 0) {}

    /* ps ax для вывода всех процессов
     * ls | cat 
     * gdb /bin/cat <pid cat>
     * bt -- посмотреть стек
     * ls -l /proc/<pid proc>/fd -- посмотреть файловые дескрипторы */
}