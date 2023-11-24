#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <errno.h>
#include "parser.h"
#include "interpreter.h"

#define EXIT_CODE_LEN 12

enum { 
    INP_ERR, 
    FORK_ERR, 
    OPEN_ERR, 
    EXEC_ERR,
    UNKNOWN_PROG,
	INFILE_ERR,
	OUTFILE_ERR,
    INTERN_ERR
};

static int background_cnt = 0; /* background process counter */
static int status; /* status of ended process*/
static process_list foreground = NULL; /* PIDs to wait() there */

static int execute_list(tree cmd, process_list *zombie_list);
static int execute_pipe(tree cmd, process_list *zombie_list);
static void execute_cmd(tree cmd, int is_piped);
static int execute_cd(tree cmd);
static int execute_exit(tree cmd, process_list *zombie_list);

// и тут нужно освобождать ресурсы
static void error(int error_code)
{
	switch (error_code) {
		case INFILE_ERR:
			fprintf(stderr, "Input file opening error\n");
			break;

		case OUTFILE_ERR:
			fprintf(stderr, "Output file opening error\n");
			break;

		default:
			fprintf(stderr, "Error in command executing\n");
			break;
	}

	exit(1);
}

void clear_zombie(process_list zombie_list, int level)
{
	if (level == 0)
		return;

	process zombie_processes = { 0, 0, NULL, zombie_list };
	process_list del, zombie = &zombie_processes;

	while (zombie->next != NULL) {
		kill(zombie->next->pid, SIGKILL);
		del = zombie->next;
		zombie->next = zombie->next->next;
		if (del->name != NULL)
			free(del->name);
		free(del);
	}

	clear_zombie(foreground, level - 1);
}

/* Ожидание завершения всех текущих процессов */
void wait_foreground_processes(process_list *foreground_list, int *exit_code)
{
    process foreground_processes = {0, 0, NULL, *foreground_list};
    process_list del, foreground_process = &foreground_processes;

    while (foreground_process->next != NULL) {
        if (waitpid(foreground_process->next->pid, &status, 0)) {
            if (foreground_process->next->next == NULL) {
                if (WIFEXITED(status)) {
                    *exit_code = WEXITSTATUS(status);
                } else {
                    *exit_code = 1;
                }
            }
            del = foreground_process->next;
            foreground_process->next = foreground_process->next->next;
            if (del->name != NULL) {
                free(del->name);
            }
            free(del);
            *foreground_list = foreground_processes.next;
        } else {
            foreground_process = foreground_process->next;
        }
    }
}

/* Ожидание завершения всех процессов-зомби */
process_list wait_zombies(process_list zombie_list)
{
    process zombie_processes = { 0, 0, NULL, zombie_list };
    process_list del, zombie = &zombie_processes;
    int i;

    // Поиск и удаление завершенных процессов-зомби
    while (zombie->next != NULL) {
        if (waitpid(zombie->next->pid, &status, WNOHANG)) {
            printf("[%d] %d %s finished\n", zombie->next->cnt, zombie->next->pid, zombie->next->name);
            del = zombie->next;
            zombie->next = zombie->next->next;
            if (del->name != NULL) {
                free(del->name);
            }
            free(del);
        } else {
            zombie = zombie->next;
        }
    }

    // Ожидание завершения всех текущих процессов
    wait_foreground_processes(&foreground, &i);

    return zombie_processes.next;
}

process_list clear_process_list(process_list list)
{
    if (list == NULL) {
        return NULL;
    }

    clear_process_list(list->next);
    if (list->name != NULL) {
        free(list->name);
    }
    free(list);

    return NULL;
}

void execute(tree cmd, process_list *zombie_list) //
{
    int exit_code = 1;

    if (cmd != NULL) {
        exit_code = execute_list(cmd, zombie_list);
    }

	char buf[EXIT_CODE_LEN];
	snprintf(buf, sizeof(buf), "%d", exit_code);
    setenv("?", buf, 1);
}

static int execute_list(tree cmd, process_list *zombie_list)
{
    int exit_code = 0, is_execute = 1;

    while (cmd != NULL) {
        if (is_execute == 1) {
            exit_code = execute_pipe(cmd, zombie_list);
        }
        switch (cmd->type) {
            case NEXT:
                is_execute = 1;
                break;

            case AND:
                (exit_code == 0) ? (is_execute = 1) : (is_execute = 0); //
                break;

            case OR:
                (exit_code == 0) ? (is_execute = 0) : (is_execute = 1); //
                break;
        }
        cmd = cmd->next;
    }

    return exit_code;
}

static process_list add_pid(pid_t pid, int cnt, char *name, process_list zombie_list)
{
    process last_struct = { 0, 0, NULL, zombie_list };
    process_list last = &last_struct;

    while (last->next != NULL) {
        last = last->next;
    }

    if ((last->next = malloc(sizeof(process))) == NULL) {
        fprintf(stderr, "Can't allocate dynamic memory for zombie pid=%d storing\n", pid); 
        return last_struct.next;       
    } 

    last = last->next;
    last->next = NULL;
    last->pid = pid;
    last->cnt = cnt;

    if (name == NULL) {
        last->name = NULL;
    } else {
		if ((last->name = calloc(strlen(name) + 1, sizeof(char))) == NULL) {
			fprintf(stderr, "Can't allocate dynamic memory for [%d]'s name\n", cnt);
        } else {
			strcpy(last->name, name);    
        }    
    }

    return last_struct.next;
}

static int execute_pipe(tree cmd, process_list *zombie_list)
{
	int fd[2], in, out, next_in;
	int exit_code = 0, is_wait_for_exit = 0;
	pid_t pid, last_pid = 0;

    // Если следующий конвейер связан с текущим по || или &&, нужно дождаться завершения последнего процесса, даже если он фоновый.
    if (cmd->type != NEXT && cmd->background == 1) {
        is_wait_for_exit = 1;
    }

    if (cmd->pipe == NULL) {
        if ((cmd->argv[0] != NULL) && (strcmp(cmd->argv[0], "cd") == 0)) {
            return execute_cd(cmd);
        }
        if ((cmd->argv[0] != NULL) && (strcmp(cmd->argv[0], "exit") == 0)) {
            return execute_exit(cmd, zombie_list);
        }
        // Создание нового процесса для выполнения команды
        if ((pid = fork()) == 0) {
            execute_cmd(cmd, 0);
        } else {
            // add_pid
            if (cmd->background == 1) {
                if (zombie_list != NULL) {
                    fprintf(stderr, "[%d] %d\n", ++background_cnt, pid);
                    last_pid = pid;
                    *zombie_list = add_pid(pid, background_cnt, cmd->argv[0], *zombie_list);
                } else {
                    foreground = add_pid(pid, 0, cmd->argv[0], foreground);
                }
            }
        }
    } else {
		pipe(fd);
		out = fd[1];
		next_in = fd[0];

		/* first process */
		if ((pid = fork()) == 0) {
			close(next_in);
			dup2(out, 1);
			close(out);
			execute_cmd(cmd, 0);
		}

        // add_pid
        if (cmd->background == 1) {
            if (zombie_list != NULL) {
                fprintf(stderr, "[%d] %d\n", ++background_cnt, pid);
                last_pid = pid;
                *zombie_list = add_pid(pid, background_cnt, cmd->argv[0], *zombie_list);
            } else {
                foreground = add_pid(pid, 0, cmd->argv[0], foreground);
            }
        }
		
		in = next_in;
		
		for (cmd = cmd->pipe; cmd->pipe != NULL; cmd = cmd->pipe) {
			close(out);
			pipe(fd);
			out = fd[1];
			next_in = fd[0];
			if ((pid = fork()) == 0) {
				close(next_in);
				dup2(in, 0);
				close(in);
				dup2(out, 1);
				close(out);
				execute_cmd(cmd, 1);
			}
			close(in);

            // add_pid
            if (cmd->background == 1) {
                if (zombie_list != NULL) {
                    fprintf(stderr, "[%d] %d\n", ++background_cnt, pid);
                    last_pid = pid;
                    *zombie_list = add_pid(pid, background_cnt, cmd->argv[0], *zombie_list);
                } else {
                    foreground = add_pid(pid, 0, cmd->argv[0], foreground);
                }
            }

			in = next_in;
		}
		
		close(out);

		/* last process */
		if ((pid = fork()) == 0) {
			dup2(in, 0);
			close(in);
			execute_cmd(cmd, 1);
		}
		
        // add_pid
        if (cmd->background == 1) {
            if (zombie_list != NULL) {
                fprintf(stderr, "[%d] %d\n", ++background_cnt, pid);
                last_pid = pid;
                *zombie_list = add_pid(pid, background_cnt, cmd->argv[0], *zombie_list);
            } else {
                foreground = add_pid(pid, 0, cmd->argv[0], foreground);
            }
        }

		close(in);
	}

    // Ожидание фоновых зомби
    wait_foreground_processes(&foreground, &exit_code);

    if (is_wait_for_exit == 1) {
        // Ожидание завершения последнего процесса, если необходимо
        waitpid(last_pid, &status, 0);
        if (WIFEXITED(status)) {
            exit_code = WEXITSTATUS(status);
        } else {
            exit_code = 1;
        }
    }

    return exit_code;
}

static int execute_cd(tree cmd)
{
	if (cmd->argv[1] == NULL)
		return chdir(getenv("HOME"));
	else
		return chdir(cmd->argv[1]);
}


// вообще тут нужно освобождать ресурсы
static int execute_exit(tree cmd, process_list *zombie_list)
{
	int exit_code = 0;

	clear_zombie(*zombie_list, 2);
	if (cmd->argv[1] != NULL)
		exit_code = atoi(cmd->argv[1]); //?

	exit(exit_code);
}

static void execute_cmd(tree cmd, int is_piped)
{
    if ((cmd->argv[0] != NULL) && ((strcmp(cmd->argv[0], "cd") == 0) || (strcmp(cmd->argv[0], "exit") == 0))) {
        exit(0);
    }

    int fd_in = 0, fd_out = 0;

    if (cmd->infile != NULL) {
        if ((fd_in = open(cmd->infile, O_RDONLY)) == -1) {
            fprintf(stderr, "open() failed: %s\n", strerror(errno));
            error(INFILE_ERR);
        }
    }

    if (cmd->outfile != NULL) {
        if (cmd->append == 1) {
            fd_out = open(cmd->outfile, O_WRONLY | O_CREAT | O_APPEND, 0666);
        } else {
            fd_out = open(cmd->outfile, O_WRONLY | O_CREAT | O_TRUNC, 0666);
        }
        if (fd_out == -1) {
            fprintf(stderr, "open() failed: %s\n", strerror(errno));
            error(OUTFILE_ERR);
        }
    }

    // Перенаправление ввода/вывода
    if (fd_in == 0 && is_piped == 0 && cmd->background == 1) {
        if ((fd_in = open("/dev/null", O_RDONLY)) == -1) {
            fprintf(stderr, "open() failed: %s\n", strerror(errno));
            error(INTERN_ERR);            
        }
    }

    if (fd_in) {
        dup2(fd_in, 0);
        close(fd_in);
    }

    if (fd_out) {
        dup2(fd_out, 1);
        close(fd_out);
    }

    signal(SIGINT, cmd->background ? SIG_IGN : SIG_DFL);

    if (cmd->psubcmd != NULL) {
        foreground = NULL;
        int exit_code = execute_list(cmd->psubcmd, NULL);
        exit(exit_code);
    } else {
        if (cmd->argv[0] != NULL) {
            execvp(cmd->argv[0], cmd->argv);
        } else {
            exit(0);
        }
        error(EXEC_ERR);
        exit(EXEC_ERR);
    }
}