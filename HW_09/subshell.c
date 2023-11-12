#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>

enum { INP_ERR, FORK_ERR, EXEC_ERR, PIPE_ERR };

/* ((pr1 | pr2); pr3) | pr4 */

int main(int argc, char **argv)
{
    pid_t pid_sub1, pid_sub2, pid1, pid2, pid3, pid4;
    int fds1[2] = {0}, fds2[2] = {0}; 

    if (argc != 5) {
        fprintf(stderr, "usage: %s <pr1> <pr2> <pr3> <pr4>\n", argv[0]);
        exit(INP_ERR);
    }

    /* create pipe: subshell2 | pr4 */
    if (pipe(fds2) < 0) {
        fprintf(stderr, "pipe() failed: %s\n", strerror(errno));
        exit(PIPE_ERR);
    }

    /* subshell 1: ((pr1 | pr2); pr3) */
    if ((pid_sub1 = fork()) < 0) {
        fprintf(stderr, "fork() failed: %s\n", strerror(errno));
        exit(FORK_ERR);          
    } else if (pid_sub1 == 0) {
        close(fds2[0]);
        /* create pipe: pr1 | pr2 */
        if (pipe(fds1) < 0) {
            fprintf(stderr, "pipe() failed: %s\n", strerror(errno));
            exit(PIPE_ERR);
        }
        /* subshell 2: (pr1 | pr2) */
        if ((pid_sub2 = fork()) < 0) {
            fprintf(stderr, "fork() failed: %s\n", strerror(errno));
            exit(FORK_ERR);    
        } else if (pid_sub2 == 0) {
            /* pr1 */
            if ((pid1 = fork()) < 0) {
                fprintf(stderr, "fork() failed: %s\n", strerror(errno));
                exit(FORK_ERR);        
            } else if (pid1 == 0) {
                dup2(fds1[1], 1);
                close(fds1[0]);
                close(fds1[1]);
                close(fds2[1]);
                execlp(argv[1], argv[1], NULL);
                fprintf(stderr, "exec() failed: %s\n", strerror(errno));
                _exit(EXEC_ERR);       
            }        
            close(fds1[1]);
            /* pr2 */
            if ((pid2 = fork()) < 0) {
                fprintf(stderr, "fork() failed: %s\n", strerror(errno));
                exit(FORK_ERR);   
            } else if (pid2 == 0) {
                dup2(fds1[0], 0);
                dup2(fds2[1], 1);
                close(fds1[0]);
                close(fds2[1]);
                execlp(argv[2], argv[2], NULL);
                fprintf(stderr, "exec() failed: %s\n", strerror(errno));
                _exit(EXEC_ERR);  
            }
            close(fds1[0]);
            close(fds2[1]);
            wait(NULL);
            wait(NULL);
        }
        close(fds1[0]);
        close(fds1[1]);
        wait(NULL);
        /* pr3 */
        if ((pid3 = fork()) < 0) {
            fprintf(stderr, "fork() failed: %s\n", strerror(errno));
            exit(FORK_ERR);   
        } else if (pid3 == 0) {
            dup2(fds2[1], 1);
            close(fds2[1]);
            execlp(argv[3], argv[3], NULL);
            fprintf(stderr, "exec() failed: %s\n", strerror(errno));
            _exit(EXEC_ERR);  
        }
        close(fds2[1]);
        wait(NULL);
    }

    close(fds2[1]);
    wait(NULL);

    /* pr4 */
    if ((pid4 = fork()) < 0) {
        fprintf(stderr, "fork() failed: %s\n", strerror(errno));
        exit(FORK_ERR);   
    } else if (pid4 == 0) {
        dup2(fds2[0], 0);
        close(fds2[0]);
        execlp(argv[4], argv[4], NULL);
        fprintf(stderr, "exec() failed: %s\n", strerror(errno));
        _exit(EXEC_ERR);  
    }

    close(fds2[0]);
    wait(NULL); /* wait pr4 */

    return 0;
}