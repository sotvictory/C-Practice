#include <stdio.h>
#include <stdlib.h>

typedef struct tNode *tree;
typedef struct tNode {
        int key;
        tree left;
        tree right;
} tnode;


void tree_print(tree t)
{
    if (t == NULL)
        return;

    tree_print(t->left);
    printf("%d ", t->key);
    tree_print(t->right);
} 

void tree_destruct(tree t)
{
    if (t == NULL)
        return;

    tree_destruct(t->left);
    tree_destruct(t->right);
    free(t);
}

void tree_add_node(tree *t, int number)
{
    if (!*t) {
        *t = malloc(sizeof(tnode));
        (*t)->key = number;
        (*t)->left = (*t)->right = NULL;
    }

    if (number < (*t)->key)
        tree_add_node(&(*t)->left, number);
    if (number > (*t)->key)
        tree_add_node(&(*t)->right, number);

    return;
}

int tree_search_node(tree t, int number)
{
    if (t == NULL)
        return 0;

    if (t->key == number)
        return 1;

    if (number > t->key)
        return(tree_search_node(t->right, number));
    else
        return(tree_search_node(t->left, number));
}

int tree_max_node(tree t)
{
    while (t && t->right != NULL)
        t = t->right;

    return t->key;
}

void tree_destruct_node(tree *t, int number)
{
    tree q;
    int max;

    if (*t == NULL)
        return;

    if ((*t)->key == number) {
        /* a leaf */
        if ((*t)->left==NULL && (*t)->right==NULL) {
            q = *t;
            *t = NULL;
            free(q);
            return;
        /* there is only right subtree */
        } else if ((*t)->left==NULL) {
            q = *t;
            *t = q->right;
            free(q);
            return ;
        /* there is only lest subtree */
        } else if ((*t)->right==NULL) {
            q = *t;
            *t = q->left;
            free(q);
            return ;
        /* there are two subtree */
        } else {
            (*t)->key = max = tree_max_node((*t)->left);
            tree_destruct_node(&(*t)->left, max);
            return;
        }
    }

    if (number > (*t)->key)
        tree_destruct_node(&(*t)->right, number);
    else
        tree_destruct_node(&(*t)->left, number);
}

int main(void)
{
    tree search_tree = NULL;

    tree_add_node(&search_tree, 3);
    tree_add_node(&search_tree, -10);
    tree_add_node(&search_tree, 5);
    tree_add_node(&search_tree, 2);
    tree_add_node(&search_tree, -5);

    tree_print(search_tree);
    putchar('\n');

    return 0;
}