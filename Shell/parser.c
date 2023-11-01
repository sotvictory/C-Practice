#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "lexer.h"
#include "parser.h"

#define PRINT_SHIFT 5
#define MEMORY_FAILURE -1

static tree get_cmd_lst(list lst, int size_lst);
static tree get_pipe(list lst, int size_lst);
static tree get_cmd(list lst, int size_lst);

// TODO: error handling
void mem_err(void)
{
    fprintf(stderr, "Memory allocation did not occur. Exit\n");
    exit(MEMORY_FAILURE);
}

void make_shift(int shift)
{
    while (shift--)
        fprintf(stderr, " ");
}

void print_argv(list args, int shift)
{
    for (int i = 0; args[i] != NULL; i++) {
        make_shift(shift);
        fprintf(stderr, "argv[%d] = %s\n", i, args[i]);
    }
}

void print_tree(tree cmd, int shift)
{
    if (cmd == NULL)
        return;

    if (cmd->argv != NULL) {
        print_argv(cmd->argv, shift);
    } else {
        make_shift(shift);
        fprintf(stderr, "psubshell\n");
    }

    make_shift(shift);
    fprintf(stderr, "infile = %s\n", cmd->infile == NULL ? "NULL" : cmd->infile);
    make_shift(shift);
    fprintf(stderr, "outfile = %s\n", cmd->outfile == NULL ? "NULL" : cmd->outfile);
    make_shift(shift);
    fprintf(stderr, "append = %d\n", cmd->append);
    make_shift(shift);
    fprintf(stderr, "background = %d\n", cmd->background);
    make_shift(shift);
    fprintf(stderr, "connection type = %s\n", cmd->type == NEXT ? "NEXT" : cmd->type == OR ? "OR" : "AND");

    if (cmd->psubcmd != NULL) {
        make_shift(shift);
        fprintf(stderr, "psubcmd--->\n");
        print_tree(cmd->psubcmd, shift + PRINT_SHIFT);
    }

    if (cmd->pipe != NULL) {
        make_shift(shift);
        fprintf(stderr, "pipe--->\n");
        print_tree(cmd->pipe, shift + PRINT_SHIFT);
    }

    if (cmd->next != NULL) {
        make_shift(shift);
        fprintf(stderr, "next--->\n");
        print_tree(cmd->next, shift + PRINT_SHIFT);
    }
}

tree clear_tree(tree cmd)
{
	if (cmd == NULL)
		return NULL;

	clear_tree(cmd->psubcmd);
	clear_tree(cmd->pipe);
	clear_tree(cmd->next);
	free(cmd->argv);
	free(cmd);

	return NULL;
}

static lexeme get_lex(list lst, int size_lst)
{
    static int read_lexemes = 0;

	return (read_lexemes < size_lst - 1) ? lst[read_lexemes++] : NULL;
}

void set_background(tree cmd, int is_same_command) 
{
    if (cmd == NULL)
        return;

    /* Find the last command in the pipeline */
    if (is_same_command == 1) {
        tree cur_cmd = cmd;
        while (cur_cmd->next != NULL) {
            if (cur_cmd->type == NEXT)
                cmd = cur_cmd->next;
            cur_cmd = cur_cmd->next;
        }
    }

    /* Set the background flag for the current command */
    cmd->background = 1;

    /* Set the background flag for sub-commands */
    set_background(cmd->pipe, 0);
    set_background(cmd->psubcmd, 0);
    if (cmd->type != NEXT)
        set_background(cmd->next, 0);
}

/* <cmd_list> ::= <pipe> {["&" | "&&" | "||" | ";"] <pipe>} ["&" | ";"] */
static tree get_cmd_lst(list lst, int size_lst)
{
    tree pipe, last_pipe;
    lexeme sep;

    if ((last_pipe = pipe = get_pipe(lst, size_lst)) == NULL)
        return NULL;

    while ((sep = get_lex(lst, size_lst)) != NULL) {
        if (strcmp(sep, "&") == 0) {
            last_pipe->type = NEXT;
            set_background(pipe, 1);
        } else if (strcmp(sep, ";") == 0) {
            last_pipe->type = NEXT;
        } else if (strcmp(sep, "&&") == 0) {
            last_pipe->type = AND;
        } else if (strcmp(sep, "||") == 0) {
            last_pipe->type = OR;
        } else {
            //TODO: errors
        }
        if (last_pipe->next != NULL)
            last_pipe = last_pipe->next;
    }

    return pipe;
}

/* <pipe> ::= <cmd> {"|" <cmd>} */
static tree get_pipe(list lst, int size_lst)
{
    tree pipe, last_cmd;
    lexeme sep;

    if ((last_cmd = pipe = get_cmd(lst, size_lst)) == NULL)
        return NULL;

    while ((sep = get_lex(lst, size_lst)) != NULL) {
        if (!strcmp(sep, "&") || !strcmp(sep, "&&") || !strcmp(sep, "||") || !strcmp(sep, ";")) {
            break;
        } else {
            // TODO: errors
        }
        last_cmd = last_cmd->pipe;
    }

    return pipe;
}

void null_cmd(tree *cmd)
{
    (*cmd)->argv = NULL;
    (*cmd)->infile = NULL;
    (*cmd)->outfile = NULL;
    (*cmd)->append = 0;
    (*cmd)->background = 0;
    (*cmd)->psubcmd = NULL;
    (*cmd)->pipe = NULL;
    (*cmd)->next = NULL;
    (*cmd)->type = NEXT;
}

/* <cmd> ::= (<cmd_list>) <io_redireсtion> */
static tree get_cmd(list lst, int size_lst)
{
    tree cmd;
    lexeme lex;

    if ((cmd = malloc(sizeof(cmd_inf))) == NULL)
        mem_err();
    null_cmd(&cmd);

    while ((lex = get_lex(lst, size_lst)) != NULL) {
        /* subshell */
        if (strcmp(lex, "(") == 0) {
            // subshell: get_cmd_list
        } else if (strcmp(lex, ")") == 0){
            // subshell: check correct input
        /* Input redirection */
        } else if (strcmp(lex, "<") == 0) {
            cmd->infile = get_lex(lst, size_lst);
        /* Output redirection: append mode */
        } else if (strcmp(lex, ">>") == 0) {
            cmd->outfile = get_lex(lst, size_lst);
            cmd->append = 1;
        /* Output redirection: write mode */
        } else if (strcmp(lex, ">") == 0) {
            cmd->outfile = get_lex(lst, size_lst);
		}
    }
    
    return cmd;
}

tree build_tree(list lst, int size_lst)
{
    //int brackets_cnt = 0, cur_cmd = 0;

	tree t = get_cmd_lst(lst, size_lst);

    return t;
}