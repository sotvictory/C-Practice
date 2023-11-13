#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "lexer.h"
#include "parser.h"

#define BUILD_TREE_ERR 1
#define MEM_ERR 2
#define SEP_ERR 3
#define BRACKETS_ERR 4
#define SUBSHELL_ERR 5
#define L_BRACKET_ERR 6
#define R_BRACKET_ERR 7
#define NO_INPFILE_ERR 8
#define TWO_INFILE_ERR 9
#define NO_OUTFILE_ERR 10
#define TWO_OUTFILE_ERR 11
#define EMPTY_CMD_ERR 12

static tree get_cmd_lst(list lst, int size_lst, int *brackets_cnt, int *plex);
static tree get_pipe(list lst, int size_lst, int *brackets_cnt, int *plex);
static tree get_cmd(list lst, int size_lst, int *brackets_cnt, int *plex);

static tree error(tree t, int error_code)
{
    t = clear_tree(t);

    switch (error_code) {
        case SEP_ERR:
            fprintf(stderr, "Invalid command separator\n");
            break;

        case BRACKETS_ERR:
            fprintf(stderr, "Brackets are imbalanced\n");
            break;

        case SUBSHELL_ERR:
            fprintf(stderr, "Invalid subshell call\n");
            break;

        case L_BRACKET_ERR:
            fprintf(stderr, "Invalid command: unexpected \"(\"\n");
            break;

        case R_BRACKET_ERR:
            fprintf(stderr, "Invalid command: unexpected \")\"\n");
            break;

        case TWO_INFILE_ERR:
            fprintf(stderr, "Invalid input redirection: more than 1 input file specified\n");
            break;

        case NO_INPFILE_ERR:
            fprintf(stderr, "Invalid input redirection: no input files are specified\n");
            break;

        case TWO_OUTFILE_ERR:
            fprintf(stderr, "Invalid output redirection: more than 1 output file specified\\n");
            break;

        case NO_OUTFILE_ERR:
            fprintf(stderr, "Invalid output redirection: no output files are specified\n");
            break;

        case EMPTY_CMD_ERR:
            fprintf(stderr, "Invalid empty command\n");
            break;

        case MEM_ERR:
            fprintf(stderr, "Memory error\n");
            exit(MEM_ERR);

        default:
            fprintf(stderr, "Invalid command\n");
            break;
    }

    return TREE_ERR;
}

static void make_shift(int shift)
{
    while (shift--)
        fprintf(stderr, " ");
}

static void print_argv(list argv, int shift)
{
    int i;

    for (i = 0; argv[i] != NULL; i++) {
        make_shift(shift);
        fprintf(stderr, "argv[%d] = %s\n", i, argv[i]);
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
    fprintf(stderr, "next type = %s\n", cmd->type == NEXT ? "NEXT" : cmd->type == OR ? "OR" : "AND" );

    if (cmd->psubcmd != NULL) {
        make_shift(shift);
        fprintf(stderr, "psubcmd--->\n");
        print_tree(cmd->psubcmd, shift + PRINT_SHIFT);
    } else {
        make_shift(shift);
        fprintf(stderr, "psubcmd = NULL\n");
    }

    if (cmd->pipe != NULL) {
        make_shift(shift);
        fprintf(stderr, "pipe--->\n");
        print_tree(cmd->pipe, shift + PRINT_SHIFT);
    } else {
        make_shift(shift);
        fprintf(stderr, "pipe = NULL\n");
    }

    if (cmd->next != NULL) {
        make_shift(shift);
        fprintf(stderr, "next--->\n");
        print_tree(cmd->next, shift + PRINT_SHIFT);
    } else {
        make_shift(shift);
        fprintf(stderr, "next = NULL\n");
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

static lexeme get_lex(list lst, int size_lst, int *plex)
{
    if ((*plex) < size_lst - 1) {
        return lst[(*plex)++];
    } else {
        return NULL;
    }
}

/* TODO: must ignore subshell, if "&" was found not in the subshell
 * ex: date; (ls -l | cat -n) >f & pwd */
static void set_background(tree cmd, int is_same_cmd)
{
    if (cmd == NULL)
        return;

    /* Find the last command in the pipeline */
    if (is_same_cmd == 1) {
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
    set_background(cmd->psubcmd, 0);
    set_background(cmd->pipe, 0);
    if (cmd->type != NEXT)
        set_background(cmd->next, 0);

    //fprintf(stderr, "set_background:\n");
    //print_tree(cmd, 5);
}

/* <cmd_list> ::= <pipe> {["&" | "&&" | "||" | ";"] <pipe>} ["&" | ";"] */
static tree get_cmd_lst(list lst, int size_lst, int *brackets_cnt, int *plex)
{
    tree pipe, last_pipe;
    lexeme sep;

    if ((last_pipe = pipe = get_pipe(lst, size_lst, brackets_cnt, plex)) == NULL) {
        return NULL;
    } else if (pipe == TREE_ERR) {
        return TREE_ERR;
    }

    while ((sep = get_lex(lst, size_lst, plex)) != NULL) {
        if (strcmp(sep, "&") == 0) {
            last_pipe->type = NEXT;
            set_background(pipe, 1);
        } else if (strcmp(sep, ";") == 0) {
            last_pipe->type = NEXT;
        } else if (strcmp(sep, "&&") == 0) {
            last_pipe->type = AND;
        } else if (strcmp(sep, "||") == 0) {
            last_pipe->type = OR;
        } else if (strcmp(sep, ")") == 0) {
			if ((*brackets_cnt) - 1 < 0)
				return error(pipe, BRACKETS_ERR);
			(*plex)--;
			break;
        } else {
            return error(pipe, SEP_ERR);
        }

        if ((last_pipe->next = get_pipe(lst, size_lst, brackets_cnt, plex)) == TREE_ERR) {
			pipe = clear_tree(pipe);
			return TREE_ERR;
        } else if (strcmp(sep, "&&") == 0 || strcmp(sep, "||") == 0) {
            return error(pipe, EMPTY_CMD_ERR);
		} else if (last_pipe->next != NULL) {
            last_pipe = last_pipe->next;
        }
    }

	if (pipe == NULL)
		return error(pipe, EMPTY_CMD_ERR);

    //fprintf(stderr, "get_cmd_lst:\n");
    //print_tree(pipe, 5);

	return pipe;
}

/* <pipe> ::= <cmd> {"|" <cmd>} */
static tree get_pipe(list lst, int size_lst, int *brackets_cnt, int *plex)
{
    tree cmd, last_cmd;
    lexeme sep;

    if ((last_cmd = cmd = get_cmd(lst, size_lst, brackets_cnt, plex)) == NULL) { 
        return NULL;
    } else if (cmd == TREE_ERR) {
        return TREE_ERR;
    }

    while ((sep = get_lex(lst, size_lst, plex)) != NULL) {
        if (strcmp(sep, "&") == 0 || strcmp(sep, "&&") == 0 || strcmp(sep, "||") == 0 || strcmp(sep, ";") == 0) {
            (*plex)--;
            break;
        } else if (strcmp(sep, ")") == 0) {
			if ((*brackets_cnt) - 1 < 0)
				return error(cmd, BRACKETS_ERR);
			(*plex)--;
			break;            
        } else if (strcmp(sep, "|") != 0) {
            return error(cmd, SEP_ERR);
        } else {
            if ((last_cmd->pipe = get_cmd(lst, size_lst, brackets_cnt, plex)) == TREE_ERR) {
                cmd = clear_tree(cmd);
                return TREE_ERR;
            }
            last_cmd = last_cmd->pipe;
            if (last_cmd == NULL)
                return error(cmd, EMPTY_CMD_ERR);
        }
    }

    //fprintf(stderr, "get_pipe:\n");
    //print_tree(cmd, 5);

    return cmd;
}

static void null_cmd(tree *cmd)
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
static tree get_cmd(list lst, int size_lst, int *brackets_cnt, int *plex)
{
    tree cmd;
    lexeme lex;
    int is_output = 0, is_input = 0, is_sub = 0, cnt_argv = 0;

    if ((lex = get_lex(lst, size_lst, plex)) == NULL)
        return NULL;

    if ((cmd = malloc(sizeof(cmd_inf))) == NULL)
        return error(cmd, MEM_ERR);

    null_cmd(&cmd);

    while (1) {
        if (lex == NULL) {
            break;
        /* subshell */
        } else if (strcmp(lex, "(") == 0) {
            /* subshell arguments are not allowed */
            if (cnt_argv != 0)
                return error(cmd, SUBSHELL_ERR);
            (*brackets_cnt)++;
            /* parse subshell */
            cmd->psubcmd = get_cmd_lst(lst, size_lst, brackets_cnt, plex);
            if (cmd->psubcmd == NULL)
                return error(cmd, EMPTY_CMD_ERR);
            else if (cmd->psubcmd == TREE_ERR)
                return TREE_ERR;
            /* check correct subshell input */
            if (get_lex(lst, size_lst, plex) == NULL)
                return error(cmd, BRACKETS_ERR);
            (*brackets_cnt)--;
            is_sub = 1;
        } else if (strcmp(lex, ")") == 0) {
            if ((*brackets_cnt) - 1 < 0)
                return error(cmd, BRACKETS_ERR);
            (*plex)--;
            break;
        /* input redirection */
        } else if (strcmp(lex, "<") == 0) {
            if (is_input == 0) {
                cmd->infile = get_lex(lst, size_lst, plex);
                if (cmd->infile == NULL)
                    return error(cmd, NO_INPFILE_ERR);
                is_input = 1;
            } else {
                return error(cmd, TWO_INFILE_ERR);
            }
        /* output redirection*/
        } else if (strcmp(lex, ">") == 0 || strcmp(lex, ">>") == 0) {
            if (is_output == 0) {
                cmd->outfile = get_lex(lst, size_lst, plex);
                if (cmd->outfile == NULL)
                    return error(cmd, NO_OUTFILE_ERR);
                is_output = 1;
                if (strcmp(lex, ">>") == 0)
                    cmd->append = 1;
            }
        /* cmd end */
        } else if (strcmp(lex, "&") == 0 || strcmp(lex, "&&") == 0 || strcmp(lex, "||") == 0 ||
                   strcmp(lex, "|") == 0 || strcmp(lex, ";") == 0) {
            (*plex)--;
            break;
        /* parse argv */
        } else {
            if (is_sub)
                return error(cmd, SUBSHELL_ERR);
            cnt_argv++;
            if ((cmd->argv = realloc(cmd->argv, sizeof(lexeme)*((cnt_argv + 1)))) == NULL)
                return error(cmd, MEM_ERR);
            cmd->argv[cnt_argv - 1] = lex;
            cmd->argv[cnt_argv] = NULL;
        }
        lex = get_lex(lst, size_lst, plex);
    }

    //fprintf(stderr, "get_cmd:\n");
    //print_tree(cmd, 5);

    return cmd;
}

tree build_tree(list lst, int size_lst)
{
    int brackets_cnt = 0, plex = 0;

    tree t = get_cmd_lst(lst, size_lst, &brackets_cnt, &plex);

    //fprintf(stderr, "build_tree:\n");
    //print_tree(t, 5);

    return t;
}