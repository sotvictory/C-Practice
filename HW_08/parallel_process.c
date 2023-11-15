#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

/* В командной строке заданы имена исполняемых файлов. 
 * Запустить их на выполнение параллельно. 
 * Напечатать число, равное количеству процессов, завершившихся нормально 
 * и с нулевым пользовательским статусом завершения процесса. 
 * Не оставлять зомби.
*/

#define INP_ERR 1
#define FORK_ERR 2
#define EXEC_ERR 3

int main(int argc, char **argv)
{
    pid_t pid_1, pid_2;
    int status_1, status_2, cnt_success = 0;

    if (argc != 3) {
        fprintf(stderr, "Invalid number of arguments. Usage: %s <file_1> <file_2>\n", argv[0]);
        exit(INP_ERR);
    }
    
    /* child process 1: <file_1> */
    if ((pid_1 = fork()) == 0) {
        execlp(argv[1], argv[1], NULL);
        perror("execlp");
        exit(EXEC_ERR);
    } else if (pid_1 == -1) {
        perror("fork");
        exit(FORK_ERR);
    }
    
    /* child process 2: <file_2> */
    if ((pid_2 = fork()) == 0) {
        execlp(argv[2], argv[2], NULL);
        perror("execlp");
        exit(EXEC_ERR);
    } else if (pid_1 == -1) {
        perror("fork");
        exit(FORK_ERR);
    }
    
    /* parent process */
    waitpid(pid_1, &status_1, 0);
    waitpid(pid_2, &status_2, 0);
    
    cnt_success += WIFEXITED(status_1) && WEXITSTATUS(status_1) == 0;
    cnt_success += WIFEXITED(status_2) && WEXITSTATUS(status_2) == 0;
    
    printf("Number of processes that terminated normally with a zero exit status: %d\n", cnt_success);
    
    return 0;
}
