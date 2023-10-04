#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>

#define BLOCK_SIZE 128  /* fixed limit on reading characters from a stream */
#define BUF_SIZE 16     /* initial limit on the number of characters per word */

typedef enum { Start, Stop, Punctuation, Word } vertex;

typedef struct Node *tree;

typedef struct Node {         
        char *key;
        int cnt;
        tree left;
        tree right;
} node;

void mem_error(void)
{
    fprintf(stderr, "OS did not give memory. Exit...\n");
    exit(1);
}

void print_tree(tree t, int max_cnt, int total_cnt)
{
    if (t == NULL)
        return;

    if ((t->cnt) == max_cnt)
        printf("%-16s %5u %8.2lf%%\n", t->key, t->cnt, (t->cnt) / (1.0 * total_cnt));

    print_tree(t->left, max_cnt, total_cnt);
    print_tree(t->right, max_cnt, total_cnt);
}

void print_tree_sorted(tree t, int max_cnt, int total_cnt) 
{
    while (max_cnt > 0) {
        print_tree(t, max_cnt, total_cnt);
        max_cnt--;
    }
}

void destruct_tree(tree t)
{
    if (t == NULL)
        return;
    else
        free(t->key);

    destruct_tree(t->left);
    destruct_tree(t->right);
    free(t);
}

/* there is the side effect: increase word count by 1 if the word was found */
int search_node(tree *t, const char *word, int *max_cnt)
{
    if (*t == NULL)
        return 0;

    int cmp = strcmp(((*t)->key), word);

    if (cmp == 0) {
        ((*t)->cnt)++;
        if (((*t)->cnt) > *max_cnt)
            *max_cnt = (*t)->cnt;
        return 1;
    }

    if (cmp < 0)
        return(search_node(&(*t)->left, word, max_cnt));
    else
        return(search_node(&(*t)->right, word, max_cnt));
}

void add_node(tree *t, const char *word)
{
    if (*t == NULL) {
        *t = (node *) malloc(sizeof(node));
        if (*t == NULL)
            mem_error();
        (*t)->key = strdup(word);
        (*t)->cnt = 1;
        (*t)->left = (*t)->right = NULL;
    }

    if (strcmp((*t)->key, word) < 0)
        add_node(&(*t)->left, word);
    if (strcmp((*t)->key, word) > 0)
        add_node(&(*t)->right, word);

    return;
}

void null_buf(char **buf, int *size_buf, int *cur_buf)
{
    *buf = NULL;
    *size_buf = 0;
    *cur_buf = 0;
}

char get_sym(char *stream_buf, int *pos, int *remaining_chars) 
{
    char c;

    if (*remaining_chars == 0) {
        *pos = 0;
        *remaining_chars = read(0, stream_buf, BLOCK_SIZE);
    }

    if (*remaining_chars > 0) {
        (*remaining_chars)--;
        c = stream_buf[*pos];
        (*pos)++;
    } else {
        c = EOF;
    }
    
    return c;
}

void add_sym(char **buf, int *size_buf, int *cur_buf, char c)
{
    if (*cur_buf > *size_buf - 1) {
        *buf = realloc(*buf, *size_buf += BUF_SIZE);
        if (*buf == NULL)
            mem_error();
    }
    (*buf)[*cur_buf] = c;
    (*cur_buf)++;
}

int is_sep(int c)
{
    return isspace(c) || isblank(c) || c == ',' || c == ';' || c == ':' || c == '.';
}

int is_word_char(int c)
{
    return isalpha(c) || c == '-' || c == '\'';
}

int main(void) 
{
    tree t = NULL;
    char *buf = NULL;
    char stream_buf[BLOCK_SIZE] = {0};
    int size_buf = 0, cur_buf = 0, max_cnt = 0, total_cnt = 0, pos = 0, remaining_chars = 0, c;

    vertex V = Start;
    c = get_sym(stream_buf, &pos, &remaining_chars);

    while (1)
        switch(V) {
            case Start:
                if (is_sep(c)) {
                    c = get_sym(stream_buf, &pos, &remaining_chars);
                } else if (c == EOF) {
                    print_tree_sorted(t, max_cnt, total_cnt);
                    destruct_tree(t);
                    V = Stop;
                } else {
                    null_buf(&buf, &size_buf, &cur_buf);
                    add_sym(&buf, &size_buf, &cur_buf, c);
                    if (ispunct(c)) {
                        V = Punctuation;
                    } else {
                        V = Word;
                    }
                    c = get_sym(stream_buf, &pos, &remaining_chars);
                }
                break;

            case Punctuation:
                V = Start;
                total_cnt++;
                if (search_node(&t, buf, &max_cnt) == 0)
                    add_node(&t, buf);
                free(buf);     
                break;

            case Word:
                if (is_word_char(c)) {
                    add_sym(&buf, &size_buf, &cur_buf, c);
                    c = get_sym(stream_buf, &pos, &remaining_chars);
                } else {
                    V = Start;
                    total_cnt++;
                    if (search_node(&t, buf, &max_cnt) == 0)
                        add_node(&t, buf);
                    free(buf);  
                }
                break;

            case Stop:
                exit(0);
                break;
        }

    return 0;
}