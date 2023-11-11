#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>

enum { INP_ERR, FORK_ERR, OPEN_ERR, EXEC_ERR };

/* ((pr1 | pr2 ); pr3) | pr4 */

int main(int argc, char **argv)
{
    pid_t pid;
    int fds1[2] = {0}, fds2[2] = {0}, fd0, fd1;  

    if (argc != 5) {
        fprintf(stderr, "usage: %s <pr1> <pr2> <pr3> <pr4>\n", argv[0]);
        exit(INP_ERR);
    }

    fd0 = dup(0);
    fd1 = dup(1);
    pipe(fds1);
    dup2(fds1[1], 1);

    /* pr1 */
    if ((pid = fork()) < 0) {
        fprintf(stderr, "fork() failed: %s\n", strerror(errno));
        exit(FORK_ERR);        
    } else if (pid == 0) {
        execlp(argv[1], argv[1], NULL);
        fprintf(stderr, "exec() failed: %s\n", strerror(errno));
        _exit(EXEC_ERR);       
    }

    close(fds2[1]);
    dup2(fds2[0],0);

    pipe(fds1);
    dup2(fds1[1],1);

    /* pr2 */
    if ((pid = fork()) < 0) {
        fprintf(stderr, "fork() failed: %s\n", strerror(errno));
        exit(FORK_ERR);   
    } else if (pid == 0) {
        execlp(argv[2], argv[2], NULL);
        fprintf(stderr, "exec() failed: %s\n", strerror(errno));
        _exit(EXEC_ERR);  
    }

    close(fds2[0]);
    dup2(fd0,0);

    while (wait(NULL) > 0) {}

    /* pr3 */
    if ((pid = fork()) < 0) {
        fprintf(stderr, "fork() failed: %s\n", strerror(errno));
        exit(FORK_ERR);   
    } else if (pid == 0) {
        execlp(argv[3], argv[3], NULL);
        fprintf(stderr, "exec() failed: %s\n", strerror(errno));
        _exit(EXEC_ERR);  
    }

    close(fds1[1]);
    dup2(fd1,1);
    dup2(fds1[0],0);

    /* pr4 */
    if ((pid = fork()) < 0) {
        fprintf(stderr, "fork() failed: %s\n", strerror(errno));
        exit(FORK_ERR);   
    } else if (pid == 0) {
        execlp(argv[4], argv[4], NULL);
        fprintf(stderr, "exec() failed: %s\n", strerror(errno));
        _exit(EXEC_ERR);  
    }

    close(fds1[0]);
    
    wait(NULL);

    return 0;
}