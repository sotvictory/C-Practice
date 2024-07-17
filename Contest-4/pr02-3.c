#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>

/* WARNING: errors are not processed */

/* {cmd1 || cmd2} && cmd3 
 * cmd1 = argv[1]
 * cmd2 = argv[2]
 * cmd3 = argv[3] */

/* a || b -> !a ? b: true
 * a && b -> a ? b: false
 * {...} is not subshell, it is just grouping */

int main(int argc, char **argv)
{
    pid_t pid;
    int status;

    /* cmd1 */
    if ((pid = fork()) == 0) {
        execlp(argv[1], argv[1], NULL);
    } else {
        waitpid(pid, &status, 0);
        
        if (WIFEXITED(status) == 0 || WEXITSTATUS(status) != 0) {
            /* ... || cmd2 */
            if ((pid = fork()) == 0) {
                execlp(argv[2], argv[2], NULL);
            }
            waitpid(pid, &status, 0);
        }

        if (WIFEXITED(status) != 0 && WEXITSTATUS(status) == 0) {
            /* {...} && cmd3 */
            if ((pid = fork()) == 0) {
                execlp(argv[3], argv[3], NULL);
            }
            waitpid(pid, &status, 0);
        }

        if (WIFEXITED(status) != 0 && WEXITSTATUS(status) == 0) {
            exit(0);
        } else {
            exit(1);
        }
    }
}