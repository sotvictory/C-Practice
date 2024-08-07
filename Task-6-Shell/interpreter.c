#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <errno.h>
#include "lexer.h"
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

static int background_cnt = 0;
static int status;
static process_list foreground = NULL;

static int execute_list(list *lst, int *size_lst, tree cmd, process_list *zombie_list);
static int execute_pipe(list *lst, int *size_lst, tree cmd, process_list *zombie_list);
static void execute_cmd(list *lst, int *size_lst, tree cmd, int is_piped);
static int execute_cd(tree cmd);
static int execute_exit(list *lst, int *size_lst, tree cmd, process_list *zombie_list);

void clear_resources(list *lst, int *size_lst, tree cmd)
{
    clear_list(lst, size_lst);
    clear_tree(cmd);
}

static void error(list *lst, int *size_lst, tree cmd, int error_code)
{
    clear_resources(lst, size_lst, cmd);

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

	exit(-1);
}

void clear_zombie(process_list zombie_list, int level)
{
	if (level == 0)
		return;

	process zombie_processes = {0, 0, zombie_list, NULL};
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

void wait_foreground_processes(process_list *foreground_list, int *exit_code)
{
    process foreground_processes = {0, 0, *foreground_list, NULL};
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

process_list wait_zombies(process_list zombie_list)
{
    process zombie_processes = {0, 0, zombie_list, NULL};
    process_list del, zombie = &zombie_processes;
    int i;

    while (zombie->next != NULL) {
        if (waitpid(zombie->next->pid, &status, WNOHANG)) {
            printf("[%d] + %d done\t%s\n", zombie->next->cnt, zombie->next->pid, zombie->next->name);
            fflush(stdout);
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

void execute(list *lst, int *size_lst, tree cmd, process_list *zombie_list)
{
    int exit_code = 1;

    if (cmd != NULL) {
        exit_code = execute_list(lst, size_lst, cmd, zombie_list);
    }

	char buf[EXIT_CODE_LEN];
	snprintf(buf, sizeof(buf), "%d", exit_code);
    setenv("?", buf, 1);
}

static int execute_list(list *lst, int *size_lst, tree cmd, process_list *zombie_list)
{
    int exit_code = 0, is_execute = 1;

    while (cmd != NULL) {
        if (is_execute == 1) {
            exit_code = execute_pipe(lst, size_lst, cmd, zombie_list);
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
    process last_struct = {0, 0, zombie_list, NULL};
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

static int execute_pipe(list *lst, int *size_lst, tree cmd, process_list *zombie_list)
{
	int fd[2], in, out, next_in;
	int exit_code = 0, is_wait_for_exit = 0;
	pid_t pid, last_pid = 0;

    if (cmd->type != NEXT && cmd->background == 1) {
        is_wait_for_exit = 1;
    }

    if (cmd->pipe == NULL) {
        if ((cmd->argv[0] != NULL) && (strcmp(cmd->argv[0], "cd") == 0)) {
            return execute_cd(cmd);
        }
        if ((cmd->argv[0] != NULL) && (strcmp(cmd->argv[0], "exit") == 0)) {
            return execute_exit(lst, size_lst, cmd, zombie_list);
        }

        if ((pid = fork()) == 0) {
            execute_cmd(lst, size_lst, cmd, 0);
        } else {
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
			execute_cmd(lst, size_lst, cmd, 0);
		}

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
				execute_cmd(lst, size_lst, cmd, 1);
			}
			close(in);

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
			execute_cmd(lst, size_lst, cmd, 1);
		}
		
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

    wait_foreground_processes(&foreground, &exit_code);

    if (is_wait_for_exit == 1) {
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


static int execute_exit(list *lst, int *size_lst, tree cmd, process_list *zombie_list)
{
	int exit_code = 0;

	if (cmd->argv[1] != NULL)
		exit_code = atoi(cmd->argv[1]);

    clear_resources(lst, size_lst, cmd);

	exit(exit_code);
}

static void execute_cmd(list *lst, int *size_lst, tree cmd, int is_piped)
{
    if ((cmd->argv[0] != NULL) && ((strcmp(cmd->argv[0], "cd") == 0) || (strcmp(cmd->argv[0], "exit") == 0))) {
        exit(0);
    }

    int fd_in = 0, fd_out = 0;

    if (cmd->infile != NULL) {
        if ((fd_in = open(cmd->infile, O_RDONLY)) == -1) {
            fprintf(stderr, "open() failed: %s\n", strerror(errno));
            error(lst, size_lst, cmd, INFILE_ERR);
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
            error(lst, size_lst, cmd, OUTFILE_ERR);
        }
    }

    if (fd_in == 0 && is_piped == 0 && cmd->background == 1) {
        if ((fd_in = open("/dev/null", O_RDONLY)) == -1) {
            fprintf(stderr, "open() failed: %s\n", strerror(errno));
            error(lst, size_lst, cmd, INTERN_ERR);           
        }
    }

    if (fd_in != 0) {
        dup2(fd_in, 0);
        close(fd_in);
    }

    if (fd_out != 0) {
        dup2(fd_out, 1);
        close(fd_out);
    }

    signal(SIGINT, cmd->background ? SIG_IGN : SIG_DFL);

    if (cmd->psubcmd != NULL) {
        foreground = NULL;
        int exit_code = execute_list(lst, size_lst, cmd->psubcmd, NULL);
        exit(exit_code);
    } else {
        if (cmd->argv[0] != NULL) {
            execvp(cmd->argv[0], cmd->argv);
        } else {
            exit(0);
        }
        error(lst, size_lst, cmd, EXEC_ERR);
    }
}