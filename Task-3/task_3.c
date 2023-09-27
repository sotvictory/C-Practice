#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum { MAX_LEN = 128 };

typedef struct tNode *tree;
typedef struct tNode {         
        char *key;
        unsigned int cnt;
        tree left;
        tree right;
} tnode;

void print_tree(tree t, unsigned int word_cnt)
{
    if (t == NULL)
        return;

    print_tree(t->left, word_cnt);
    printf("%-10s %5u %8.2lf%%\n", t->key, t->cnt, t->cnt / (double)word_cnt);
    print_tree(t->right, word_cnt);
}

void destruct_tree(tree t)
{
    if (t == NULL)
        return;

    destruct_tree(t->left);
    destruct_tree(t->right);
    free(t);
}

void add_node(tree *t, const char *word) 
{   
    int cmp; 

    if (*t == NULL) {
        *t = (tnode *) malloc(sizeof(tnode));
        (*t)->key = strdup(word);
        (*t)->cnt = 1;
        (*t)->left = (*t)->right = NULL;
    } else {
        cmp = strcmp(word, (*t)->key);
        if (cmp == 0) {
            (*t)->cnt++;
        } else if (cmp < 0) {
            add_node(&(*t)->left, word);
        } else {
            add_node(&(*t)->right, word);
        }
    }

    return;
}

int main(void)
{
    tree binary_tree = NULL;
    unsigned int word_cnt = 1;
    char buf[MAX_LEN] = "gfgdfgdfh";

    add_node(&binary_tree, buf);
    print_tree(binary_tree, word_cnt);
    printf("\n");

    return 0;
}