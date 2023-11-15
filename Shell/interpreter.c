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

enum { 
    INP_ERR, 
    FORK_ERR, 
    OPEN_ERR, 
    EXEC_ERR,
    UNKNOWN_PROG 
};

void error (int error_code)
{
    return;
}

void cd(tree cmd)
{
	if (cmd->argv[1] == NULL)
		chdir(getenv("HOME"));
	else
		chdir(cmd->argv[1]);
    
    return;
}

void execute_pipe(tree cmd)
{
	int fd[2], in, out, next_in;
	pid_t pid;

	pipe(fd);

	out = fd[1];
	next_in = fd[0];

	/* first process */
	if ((pid = fork())==0) {
		close(next_in);
		dup2(out, 1);
		close(out);
		execute_cmd(cmd);
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
			execute_cmd(cmd);
		}
		close(in);
		in = next_in;
	}
	
	close(out);

	/* last process */
	if ((pid = fork()) == 0) {
		dup2(in, 0);
		close(in);
		execute_cmd(cmd);
	}
	
	close(in);
}

int execute_cmd(tree cmd)
{
    pid_t pid;
    int fd_in = 0, fd_out = 0;

    if (strcmp(cmd->argv[0], "cd") == 0) {
        cd(cmd);
        return 0;
    } else if (strcmp(cmd->argv[0], "exit") == 0) {
        clear_tree(cmd);
        /* здесь надо и list почистить*/
        exit(0);
    }

	/* input redirection */
	if (cmd->infile != NULL) {
        if ((fd_in = open(cmd->infile, O_RDONLY)) < 0) {
            fprintf(stderr, "open() failed: %s\n", strerror(errno));
            _exit(OPEN_ERR);
        }
	}

    /* output redirection */
	if (cmd->outfile != NULL) {
		if (cmd->append == 1) {
            if ((fd_out = open(cmd->outfile, O_WRONLY | O_CREAT | O_APPEND, 0666)) < 0) {
                fprintf(stderr, "open() failed: %s\n", strerror(errno));
                _exit(OPEN_ERR);                
            }
        } else if (cmd->append == 0) {
            if ((fd_out = open(cmd->outfile, O_WRONLY | O_CREAT | O_TRUNC, 0666)) < 0) {
                fprintf(stderr, "open() failed: %s\n", strerror(errno));
                _exit(OPEN_ERR);                  
            }
        }
	}

	if (fd_in == 0 && cmd->background == 1) {
        if ((fd_out = open("/dev/null", O_RDONLY)) < 0) {
            fprintf(stderr, "open() failed: %s\n", strerror(errno));
            _exit(OPEN_ERR);                
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

	if (cmd->psubcmd) {
        // subshell
	} else {
		if (cmd->argv[0] != NULL) {
            if ((pid = fork()) == 0) {
                execvp(cmd->argv[0], cmd->argv);
                fprintf(stderr, "exec() failed: %s\n", strerror(errno));
                _exit(EXEC_ERR);
            } else if (pid < 0) {
                fprintf(stderr, "fork() failed: %s\n", strerror(errno));
                exit(FORK_ERR);
            } else {
                waitpid(pid, NULL, 0);
            }            
        } else {
            return 0;
        }
	}

    close(fd_in);
    close(fd_out);

    return 0;
}