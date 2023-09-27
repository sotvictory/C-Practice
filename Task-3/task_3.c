#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct Node *tree;

typedef struct Node {         
        char *key;
        unsigned int cnt;
        tree left;
        tree right;
} node;

void mem_err(void)
{
    fprintf(stderr, "OS did not give memory. Exit...\n");
    exit(1);
}

void print_tree(tree t, unsigned int max_cnt, unsigned int total_cnt)
{
    if (t == NULL)
        return;

    if ((t->cnt) == max_cnt)
        printf("%-16s %5u %8.2lf%%\n", t->key, t->cnt, (t->cnt) / (1.0 * total_cnt));

    print_tree(t->left, max_cnt, total_cnt);
    print_tree(t->right, max_cnt, total_cnt);
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

int search_node(tree *t, const char *word, unsigned int *max_cnt)
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
            mem_err();
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

int main(void) 
{
    tree t = NULL;
    int buf_size = 16, buf_cnt = 0, c;
    unsigned int max_cnt = 0, total_cnt = 0;
    char *buf = NULL;
    char ch;

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
        }

        if (!isalpha(c) && (c != '\n') && (c != ' ') && (c != EOF)) {
            ch = (char)c; 
            add_node(&t, &ch);
        }
    }

    while (max_cnt > 0) {
        print_tree(t, max_cnt, total_cnt);
        max_cnt--;
    }

    destruct_tree(t);

    return 0;
}