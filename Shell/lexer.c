#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "lexer.h"

#define BLOCK_SIZE 1024         /* fixed limit on reading characters from a stream */
#define LEXEME_SIZE 16          /* initial limit on the number of characters per lexeme */

#define MEMORY_ERR 1
#define QUOTES_ERR 2

typedef enum { 
    START, 
    WORD, 
    SINGLE_SPECIAL_CHAR, 
    DOUBLE_SPECIAL_CHAR, 
    SINGLE_OR, 
    SINGLE_AND, 
    SINGLE_GREATER, 
    QUOTE,
    SLASH,
    HASH_START,
    HASH_WORD,
    STOP
} vertex;

static void error(list *lst, int *size_lst, int error_code)
{
    switch (error_code) {
        case QUOTES_ERR:
            clear_list(lst, size_lst);
            fprintf(stderr, "Quotes are imbalanced\n");
            return;

        case MEMORY_ERR:
            fprintf(stderr, "Memory error\n");
            exit(MEMORY_ERR);
    }
}

static void null_list(list *lst, int *size_lst, int *cur_lst)
{
    *lst = NULL;
    *size_lst = 0;
    *cur_lst = 0;
}

static void null_lex(lexeme *lex, int *size_lex, int *cur_lex)
{
    *lex = NULL;
    *size_lex = 0;
    *cur_lex = 0;
}

static int sym_set(int c) 
{
    return c != '\n' && c != ' ' && c != '\t' && c != EOF && c != ';' && 
           c != '<' && c != '(' && c != ')' && c != '|' && c != '&' && c != '>';
}

void print_list(list lst, int size_lst, int output_fd)
{
    int i;

    if (lst == NULL) 
        return;

    for (i = 0; i < size_lst - 1; i++) {
        write(output_fd, lst[i], strlen(lst[i]));
        write(output_fd, "\n", 1);
    }
}

void clear_list(list *lst, int *size_lst)
{
    int i;
    *size_lst = 0;

    if (*lst == NULL) 
        return;

    for (i = 0; (*lst)[i] != NULL; i++)
        free((*lst)[i]);
    free(*lst);

    (*lst) = NULL;
}

static int get_sym(int input_fd) 
{
    static char stream_buf[BLOCK_SIZE] = {0};
    static int remaining_chars = 0, pos = 0;
    int c;

    /* read a new data block */
    if (remaining_chars == 0) {
        pos = 0;
        remaining_chars = read(input_fd, stream_buf, BLOCK_SIZE);
    }

    /* get the next character of the read data block */
    if (remaining_chars > 0) {
        remaining_chars--;
        c = stream_buf[pos++];
    } else {
        c = EOF;
    }
    
    return c;
}

static void add_sym(list *lst, int *size_lst, lexeme *lex, int *size_lex, int *cur_lex, char c)
{
    /* increase lexeme size if necessary */
    if (*cur_lex > *size_lex - 1) {
        *size_lex += LEXEME_SIZE;
        *lex = realloc(*lex, *size_lex);
        if (*lex == NULL)
            error(lst, size_lst, MEMORY_ERR);
    }

    /* add symbol to lexeme */
    (*lex)[*cur_lex] = c;
    (*cur_lex)++;
}

static void add_word(list *lst, int *size_lst, int *cur_lst, lexeme *lex, int *size_lex, int *cur_lex) 
{
    /* increase lexeme size to write '\0' if necessary */
    if (*cur_lex > *size_lex - 1) {
        *size_lex += 1;
        *lex = realloc(*lex, *size_lex);
        if (*lex == NULL)
           error(lst, size_lst, MEMORY_ERR);
    }
    (*lex)[*cur_lex] = '\0';
    (*cur_lex)++;

    /* limit memory to lexeme size */
    *size_lex = *cur_lex;
    *lex = realloc(*lex, *size_lex);
    if (*lex == NULL)
        error(lst, size_lst, MEMORY_ERR);

    /* increase list size if necessary */
    if (*cur_lst > *size_lst - 1) {
        *size_lst += LEXEME_SIZE;
        *lst = realloc(*lst, *size_lst * sizeof(list));
        if (*lst == NULL)
            error(lst, size_lst,MEMORY_ERR);
    }

    /* add lexeme to list */
    (*lst)[*cur_lst] = *lex;
    (*cur_lst)++;
}

static void term_list(list *lst, int *size_lst, int *cur_lst)
{
    if (*lst == NULL) 
        return;

    /* increase list size to write NULL if necessary */
    if (*cur_lst > *size_lst - 1) {
        *lst = realloc(*lst, (*size_lst + 1) * sizeof(list));
        if (*lst == NULL)
            error(lst, size_lst, MEMORY_ERR);
    }
    (*lst)[*cur_lst] = NULL;

    /* limit memory to list size  */
    *size_lst = *cur_lst + 1;
    *lst = realloc(*lst, *size_lst * sizeof(list));
    if (*lst == NULL)
        error(lst, size_lst, MEMORY_ERR);
}

void build_list(list *lst, int *size_lst, int input_fd, int output_fd)
{
    int cur_lst = 0, cur_lex = 0, size_lex = 0, quote_cnt = 0, c;
    lexeme lex = NULL;

    vertex V = START;
    c = get_sym(input_fd);
    null_list(lst, size_lst, &cur_lst);

    while (1) {
        switch(V) {
            case START:
                if (c == ' ' || c == '\t') {
                    c = get_sym(input_fd);
                } else if (c == EOF) {
                    term_list(lst, size_lst, &cur_lst);
                    if (quote_cnt % 2 == 1)
                        error(lst, size_lst, QUOTES_ERR);
                    V = STOP;
                } else if (c == '\n') {
                    c = get_sym(input_fd);
                } else if (c == '#') {
                    c = get_sym(input_fd);
                    V = HASH_START;
                } else if (c == '"') {
                    null_lex(&lex, &size_lex, &cur_lex);
                    quote_cnt++;
                    c = get_sym(input_fd);
                    V = QUOTE;
                } else if (c == '\\') {
                    null_lex(&lex, &size_lex, &cur_lex);
                    c = get_sym(input_fd);
                    V = SLASH;          
                } else {
                    null_lex(&lex, &size_lex, &cur_lex);
                    add_sym(lst, size_lst, &lex, &size_lex, &cur_lex, c);
                    if (c == ';' || c == '<' || c == '(' || c == ')') {
                        V = SINGLE_SPECIAL_CHAR;
                    } else if (c == '|') {
                        V = SINGLE_OR;
                    } else if (c == '&') {
                        V = SINGLE_AND;
                    } else if (c == '>') {
                        V = SINGLE_GREATER;
                    } else {
                        V = WORD;
                    }
                    c = get_sym(input_fd);
                }
                break;
            
            case HASH_START:
                if (c == '\n' || c == EOF) {
                    V = START;
                } else {
                    c = get_sym(input_fd);
                }
                break;

            case HASH_WORD:
                if (c == '\n' || c == EOF) {
                    add_word(lst, size_lst, &cur_lst, &lex, &size_lex, &cur_lex);
                    V = START;
                }
                break;

            case WORD:
                if (c == '"') {
                    quote_cnt++;
                    c = get_sym(input_fd);
                    V = QUOTE;
                    break;
                } else if (c == '\\') {
                    V = SLASH;
                    break;
                } else if (c == '#') {
                    c = get_sym(input_fd);
                    V = HASH_WORD;
                    break;
                } else if (sym_set(c)) {
                    add_sym(lst, size_lst, &lex, &size_lex, &cur_lex, c);
                    c = get_sym(input_fd);
                } else {
                    add_word(lst, size_lst, &cur_lst, &lex, &size_lex, &cur_lex);
                    V = START;
                }
                break;

            case QUOTE:
                if (c == '"') {
                    quote_cnt++;
                    c = get_sym(input_fd);
                    V = WORD;
                    break;
                } else if (c != '\n' && c != EOF) {
                    add_sym(lst, size_lst, &lex, &size_lex, &cur_lex, c);
                    c = get_sym(input_fd);
                } else {
                    add_word(lst, size_lst, &cur_lst, &lex, &size_lex, &cur_lex);
                    V = START;
                }
                break;

            case SLASH:
                if (c != '\n' && c != EOF) {
                    add_sym(lst, size_lst, &lex, &size_lex, &cur_lex, c);
                    c = get_sym(input_fd);
                    V = WORD;
                } else {
                    add_word(lst, size_lst, &cur_lst, &lex, &size_lex, &cur_lex);
                    V = START;
                }
                break;

            case SINGLE_OR:
                if (c == '|') {
                    add_sym(lst, size_lst, &lex, &size_lex, &cur_lex, c);
                    c = get_sym(input_fd);
                    V = DOUBLE_SPECIAL_CHAR;
                } else {
                    add_word(lst, size_lst, &cur_lst, &lex, &size_lex, &cur_lex);
                    V = START;
                }
                break;

            case SINGLE_AND:
                if (c == '&') {
                    add_sym(lst, size_lst, &lex, &size_lex, &cur_lex, c);
                    c = get_sym(input_fd);
                    V = DOUBLE_SPECIAL_CHAR;
                } else {
                    add_word(lst, size_lst, &cur_lst, &lex, &size_lex, &cur_lex);
                    V = START;
                }
                break;

            case SINGLE_GREATER:
                if (c == '>') {
                    add_sym(lst, size_lst, &lex, &size_lex, &cur_lex, c);
                    c = get_sym(input_fd);
                    V = DOUBLE_SPECIAL_CHAR;
                } else {
                    add_word(lst, size_lst, &cur_lst, &lex, &size_lex, &cur_lex);
                    V = START;
                }
                break;

            case SINGLE_SPECIAL_CHAR:
            case DOUBLE_SPECIAL_CHAR:
                add_word(lst, size_lst, &cur_lst, &lex, &size_lex, &cur_lex);
                V = START;
                break;

            case STOP:
                return;
        }
    }
}