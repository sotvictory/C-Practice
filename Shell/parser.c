#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "lexer.h"
#include "parser.h"

#define PRINT_SHIFT 5
#define MEMORY_FAILURE -1

void mem_error(void)
{
    fprintf(stderr, "Memory allocation did not occur. Exit\n");
    exit(MEMORY_FAILURE);
}

void make_shift(int shift)
{
    while (shift--)
        fprintf(stderr, " ");
}

void print_argv(list p, int shift)
{
    int i;

    for (i = 0; p[i] != NULL; i++) {
        make_shift(shift);
        fprintf(stderr, "argv[%d] = %s\n", i, p[i]);
    }
}

void print_tree(tree t, int shift)
{
    if (t == NULL)
        return;

    if (t->argv != NULL) {
        print_argv(t->argv, shift);
    } else {
        make_shift(shift);
        fprintf(stderr, "psubshell\n");
    }

    make_shift(shift);
    fprintf(stderr, "infile = %s\n", t->infile == NULL ? "NULL" : t->infile);
    make_shift(shift);
    fprintf(stderr, "outfile = %s\n", t->outfile == NULL ? "NULL" : t->outfile);
    make_shift(shift);
    fprintf(stderr, "append = %d\n", t->append);
    make_shift(shift);
    fprintf(stderr, "background = %d\n", t->background);
    make_shift(shift);
    fprintf(stderr, "connection type = %s\n", t->type == NXT ? "NXT" : t->type == OR ? "OR" : "AND");

    if (t->psubcmd != NULL) {
        make_shift(shift);
        fprintf(stderr, "psubcmd--->\n");
        print_tree(t->psubcmd, shift + PRINT_SHIFT);
    }

    if (t->pipe != NULL) {
        make_shift(shift);
        fprintf(stderr, "pipe--->\n");
        print_tree(t->pipe, shift + PRINT_SHIFT);
    }

    if (t->next != NULL) {
        make_shift(shift);
        fprintf(stderr, "next--->\n");
        print_tree(t->next, shift + PRINT_SHIFT);
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

lexeme get_lexeme(list lst, int size_lst)
{
    static int read_lexemes = 0;

	return (read_lexemes < size_lst - 1) ? lst[read_lexemes++] : NULL;
}

/* <cmd_list> ::= <pipe> {["&" | "&&" | "||" | ";"] <pipe>} ["&" | ";"] */
tree get_cmd_lst(list lst, int size_lst)
{
    return NULL;
}

/* <pipe> ::= <cmd> {"|" <cmd>} */
tree get_pipe(list lst, int size_lst)
{
    return NULL;
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
    (*cmd)->type = NXT;
}

/* <cmd> ::= (<cmd_list>) <io_redireсtion> */
tree get_cmd(list lst, int size_lst)
{
    tree cmd;
    lexeme lex;

    if ((cmd = malloc(sizeof(tree)) == NULL))
        mem_err();
    null_cmd(&cmd);

    while ((lex = get_lexeme(lst, size_lst)) != NULL) {
        /* subshell */
        if (!strcmp(lex, "(")) {
            // subshell: get_cmd_list
        } else if (!strcmp(lex, ")")){
            // subshell: check correct input
        /* Input redirection */
        } else if (!strcmp(lex, "<")) {
            cmd->infile = get_lexeme(lst, size_lst);
        /* Output redirection: append mode */
        } else if (!strcmp(lex, ">>")) {
            cmd->outfile = get_lexeme(lst, size_lst);
            cmd->append = 1;
        /* Output redirection: write mode */
        } else if (!strcmp(lex, ">")) {
            cmd->outfile = get_lexeme(lst, size_lst);
		}
    }
    
    return cmd;
}

tree build_tree(tree *cmd)
{
    //static brackets_cnt = 0;

    return NULL;
}
