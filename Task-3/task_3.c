#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>

#define BLOCK_SIZE 128  /* limit on the read characters number */
#define BUF_SIZE 16

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

//void print_tree_in_order();

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

/* имеет побочный эффект: увеличивает cnt на 1, если слово было найдено */
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

    if (--*remaining_chars >= 0) {
        c = stream_buf[*pos];
        (*pos)++;
        //fprintf(stderr, "Remaining_chars = %d\n", *remaining_chars);
        //fprintf(stderr, "Got the symbol %c\n", c);
    } else {
        c = EOF;
        //fprintf(stderr, "Got the EOF\n");
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
    //fprintf(stderr, "Character %c will be placed in position %d buf of %d elements\n", c, *cur_buf, *size_buf);
    (*buf)[*cur_buf] = c;
    (*cur_buf)++;
    //fprintf(stderr, "Buffer: %s\n", *buf);
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
    char stream_buf[BLOCK_SIZE];
    int size_buf = 0, cur_buf = 0, max_cnt = 0, total_cnt = 0, c, pos = 0, remaining_chars = 0;

    vertex V = Start;
    c = get_sym(stream_buf, &pos, &remaining_chars);

    while (1)
        switch(V) {
            case Start:
                if (is_sep(c)) {
                    c = get_sym(stream_buf, &pos, &remaining_chars);
                } else if (c == EOF) {
                    while (max_cnt > 0) { // выделить в отдельную функцию
                        print_tree(t, max_cnt, total_cnt);
                        max_cnt--;
                    }
                    //fprintf(stderr, "Total size is %d\n", total_cnt);
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
                }
                break;

            case Stop:
                exit(0);
                break;
        }

    #if 0
    // last implementation

    tree t = NULL;
    int buf_size = 16, buf_cnt = 0, max_cnt = 0, total_cnt = 0;
    char *buf = NULL;
    char c;

    buf = (char *) malloc(buf_size * sizeof(char));
    if (buf == NULL)
        mem_err();

    while ((c = getchar()) && c != EOF ) {
        if (isalpha(c) && (c != '\n') && (c != ' ') && (c != EOF)) {
            if (buf_cnt == buf_size) {
                buf_size = 1.5 * buf_size + 1;
                buf = (char *) realloc(buf, buf_size);
                if (buf == NULL)
                    mem_err();
            }
            buf[buf_cnt++] = c;
        } else if (buf_cnt > 0) {
            buf = (char *) realloc(buf, (buf_cnt + 1) * sizeof(char));
            if (buf == NULL)
                mem_err();
            buf[buf_cnt] = '\0';
            total_cnt++;
            if (search_node(&t, buf, &max_cnt) == 0)
                add_node(&t, buf);       
            free(buf);
            buf = NULL;
            buf_size = buf_cnt = 0;   
        } else if (!isalpha(c) && (c != '\n') && (c != ' ') && (c != EOF)) {
            buf = (char *) malloc(2 * sizeof(char));
            buf[0] = c;
            buf[1] = '\0';
            if (search_node(&t, buf, &max_cnt) == 0)
                add_node(&t, buf);
            free(buf);
        }
    }

    while (max_cnt > 0) {
        print_tree(t, max_cnt, total_cnt);
        max_cnt--;
    }

    destruct_tree(t);
    #endif

    return 0;
}