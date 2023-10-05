#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "lexer.h"

#define BLOCK_SIZE 256          /* fixed limit on reading characters from a stream */
#define LEXEME_SIZE 16          /* initial limit on the number of characters per lexeme */
#define MEMORY_FAILURE -1       /* memory allocation error code */

typedef enum { 
    Start, 
    Word, 
    SingleSpecialChar, 
    DoubleSpecialChar, 
    SingleOr, 
    SingleAnd, 
    SingleGreater, 
    Newline, 
    Quote, 
    Stop
} vertex;

void mem_error(void)
{
    fprintf(stderr, "Memory allocation did not occur. Exit\n");
    exit(MEMORY_FAILURE);
}

void null_list(list *lst, int *size_lst, int *cur_lst)
{
    *lst = NULL;
    *size_lst = 0;
    *cur_lst = 0;
}

void null_lex(lexeme *lex, int *size_lex, int *cur_lex)
{
    *lex = NULL;
    *size_lex = 0;
    *cur_lex = 0;
}

int sym_set(int c) 
{
    return c != '\n' && c != ' ' && c != '\t' && c != EOF && c != ';' && 
           c != '<' && c != '(' && c != ')' && c != '|' && c != '&' && c != '>';
}

void print_list(list *lst, int *size_lst)
{
    int i;

    if (*lst == NULL) 
        return;

    for (i = 0; i < *size_lst - 1; i++)
        printf("%s\n", (*lst)[i]);
}

void clear_list(list *lst, int *size_lst, int *cur_lst)
{
    int i;
    *size_lst = 0;
    *cur_lst = 0;

    if (*lst == NULL) 
        return;

    for (i = 0; (*lst)[i] != NULL; i++)
        free((*lst)[i]);
    free(*lst);

    (*lst) = NULL;
}

char get_sym(char *stream_buf, int *pos, int *remaining_chars) 
{
    char c;

    /* read a new data block */
    if (*remaining_chars == 0) {
        *pos = 0;
        *remaining_chars = read(0, stream_buf, BLOCK_SIZE);
    }

    /* get the next character of the read data block */
    if (*remaining_chars > 0) {
        (*remaining_chars)--;
        c = stream_buf[*pos];
        (*pos)++;
    } else {
        c = EOF;
    }
    
    return c;
}

void add_sym(lexeme *lex, int *size_lex, int *cur_lex, char c)
{
    /* increase lexeme size if necessary */
    if (*cur_lex > *size_lex - 1) {
        *size_lex += LEXEME_SIZE;
        *lex = realloc(*lex, *size_lex);
        if (*lex == NULL)
            mem_error();
    }

    /* add symbol to lexeme */
    (*lex)[*cur_lex] = c;
    (*cur_lex)++;
}

void add_word(list *lst, int *size_lst, int *cur_lst, lexeme *lex, int *size_lex, int *cur_lex) 
{
    /* increase lexeme size to write '\0' if necessary */
    if (*cur_lex > *size_lex - 1) {
        *size_lex += 1;
        *lex = realloc(*lex, *size_lex);
        if (*lex == NULL)
            mem_error();
    }
    (*lex)[*cur_lex] = '\0';
    (*cur_lex)++;

    /* limit memory to lexeme size */
    *size_lex = *cur_lex;
    *lex = realloc(*lex, *size_lex);
    if (*lex == NULL)
        mem_error();

    /* increase list size if necessary */
    if (*cur_lst > *size_lst - 1) {
        *size_lst += LEXEME_SIZE;
        *lst = realloc(*lst, *size_lst * sizeof(list));
        if (*lst == NULL)
            mem_error();
    }

    /* add lexeme to list */
    (*lst)[*cur_lst] = *lex;
    (*cur_lst)++;
}

void term_list(list *lst, int *size_lst, int *cur_lst)
{
    if (*lst == NULL) 
        return;

    /* increase list size to write NULL if necessary */
    if (*cur_lst > *size_lst - 1) {
        *lst = realloc(*lst, (*size_lst + 1) * sizeof(list));
        if (*lst == NULL)
            mem_error();
    }
    (*lst)[*cur_lst] = NULL;

    /* limit memory to list size  */
    *size_lst = *cur_lst + 1;
    *lst = realloc(*lst, *size_lst * sizeof(list));
    if (*lst == NULL)
        mem_error();
}

void build_list(list *lst, int *size_lst)
{
    char stream_buf[BLOCK_SIZE] = {0};
    int cur_lst = 0, size_lex = 0, cur_lex = 0, remaining_chars = 0, pos = 0, quote_cnt = 0, c;
    lexeme lex = NULL;
    vertex V = Start;

    c = get_sym(stream_buf, &pos, &remaining_chars);
    null_list(lst, size_lst, &cur_lst);

    while (1) {
        switch(V) {

            case Start:
                if (c == ' ' || c == '\t') {
                    c = get_sym(stream_buf, &pos, &remaining_chars);
                } else if (c == EOF) {
                    term_list(lst, size_lst, &cur_lst);
                    print_list(lst, size_lst);
                    clear_list(lst, size_lst, &cur_lst);
                    V = Stop;
                } else if (c == '\n') {
                    term_list(lst, size_lst, &cur_lst);
                    print_list(lst, size_lst);
                    c = get_sym(stream_buf, &pos, &remaining_chars);
                    V = Newline;
                } else if (c == '"') {
                    null_lex(&lex, &size_lex, &cur_lex);
                    c = get_sym(stream_buf, &pos, &remaining_chars);
                    V = Quote;                   
                } else {
                    null_lex(&lex, &size_lex, &cur_lex);
                    add_sym(&lex, &size_lex, &cur_lex, c);
                    if (c == ';' || c == '<' || c == '(' || c == ')') {
                        V = SingleSpecialChar;
                    } else if (c == '|') {
                        V = SingleOr;
                    } else if (c == '&') {
                        V = SingleAnd;
                    } else if (c == '>') {
                        V = SingleGreater;
                    } else {
                        V = Word;
                    }
                    c = get_sym(stream_buf, &pos, &remaining_chars);
                }
                break;

            case Word:
                if (c == '"') {
                    c = get_sym(stream_buf, &pos, &remaining_chars);
                    V = Quote;
                    break;
                } else if (sym_set(c)) {
                    add_sym(&lex, &size_lex, &cur_lex, c);
                    c = get_sym(stream_buf, &pos, &remaining_chars);
                } else {
                    add_word(lst, size_lst, &cur_lst, &lex, &size_lex, &cur_lex);
                    V = Start;
                }
                break;

            case Quote:
                if (c == '"') {
                    c = get_sym(stream_buf, &pos, &remaining_chars);
                    V = Word;
                    break;
                } else if (c != '\n' && c != EOF) {
                    add_sym(&lex, &size_lex, &cur_lex, c);
                    c = get_sym(stream_buf, &pos, &remaining_chars);
                } else {
                    add_word(lst, size_lst, &cur_lst, &lex, &size_lex, &cur_lex);
                    V = Start;
                }
                break;

            case SingleOr:
                if (c == '|') {
                    add_sym(&lex, &size_lex, &cur_lex, c);
                    c = get_sym(stream_buf, &pos, &remaining_chars);
                    V = DoubleSpecialChar;
                } else {
                    add_word(lst, size_lst, &cur_lst, &lex, &size_lex, &cur_lex);
                    V = Start;
                }
                break;

            case SingleAnd:
                if (c == '&') {
                    add_sym(&lex, &size_lex, &cur_lex, c);
                    c = get_sym(stream_buf, &pos, &remaining_chars);
                    V = DoubleSpecialChar;
                } else {
                    add_word(lst, size_lst, &cur_lst, &lex, &size_lex, &cur_lex);
                    V = Start;
                }
                break;

            case SingleGreater:
                if (c == '>') {
                    add_sym(&lex, &size_lex, &cur_lex, c);
                    c = get_sym(stream_buf, &pos, &remaining_chars);
                    V = DoubleSpecialChar;
                } else {
                    add_word(lst, size_lst, &cur_lst, &lex, &size_lex, &cur_lex);
                    V = Start;
                }
                break;

            case SingleSpecialChar:
            case DoubleSpecialChar:
                add_word(lst, size_lst, &cur_lst, &lex, &size_lex, &cur_lex);
                V = Start;
                break;

            case Newline:
                clear_list(lst, size_lst, &cur_lst);
                V = Start;
                break;

            case Stop:
                return;
        }
    }
}