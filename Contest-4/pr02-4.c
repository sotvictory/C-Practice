#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>

/* WARNING: errors are not processed */

/* (cmd1 < file1 && cmd2) | cmd3 >> file2 
 * cmd1 = argv[1]
 * cmd2 = argv[2]
 * cmd3 = argv[3]
 * file1 = argv[4]
 * file2 = argv[5] */

int main(int argc, char **argv)
{
    int fds[2];

    pipe(fds);

    /* subshell: (cmd1 < file1 && cmd2) */
    if (fork() == 0) {
        int status;
        pid_t pid;

        close(fds[0]);
        dup2(fds[1], 1);
        close(fds[1]);

        /* cmd1 < file1 */
        if ((pid = fork()) == 0) {
            int fd = open(argv[4], O_RDONLY);
            dup2(fd, 0);
            close(fd);
            execlp(argv[1], argv[1], NULL);
        }
        waitpid(pid, &status, 0);

        /* ... && cmd2 */
        if (WIFEXITED(status) && (WEXITSTATUS(status) == 0)) {
            if ((pid = fork()) == 0) {
                execlp(argv[2], argv[2], NULL);
            }
        }
        wait(NULL);

        exit(0);
    }

    close(fds[1]);

    /* ... | cmd3 >> file2 */
    if (fork() == 0) {
        int fd = open(argv[5], O_APPEND | O_CREAT | O_WRONLY);
        dup2(fd, 1);
        close(fd);
        dup2(fds[0], 0);
        close(fds[0]);
        execlp(argv[3], argv[3], NULL);
    }

    close(fds[0]);

    wait(NULL);
    wait(NULL);

    exit(0);
}