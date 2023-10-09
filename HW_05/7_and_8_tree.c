#include <stdio.h>
#include <stdlib.h>

typedef struct tNode *tree;

typedef struct tNode {
        int key;
        tree left;
        tree right;
} tnode;

int height(tree t)
{
    int left_height, right_height;

    if (t == NULL)
        return 0;

    left_height = height(t->left);
    right_height = height(t->right);
    if (left_height > right_height) {
        return left_height + 1;
    } else {
        return right_height + 1;
    }
}

void destruct_tree(tree t)
{
    if (t == NULL)
        return;

    destruct_tree(t->left);
    destruct_tree(t->right);

    free(t);
}

void add_node(tree *t, int number)
{
    if (!*t) {
        *t = malloc(sizeof(tnode));
        (*t)->key = number;
        (*t)->left = (*t)->right = NULL;
    } else {
        if (number < (*t)->key)
            add_node(&(*t)->left, number);
        else if (number > (*t)->key) 
            add_node(&(*t)->right, number);      
    }
}

void print_tree_layers(tree t, int level)
{
    if (t == NULL) {
        printf("NULL ");
        return;
    }

    if (level == 0) {
        printf("%d ", t->key);
    } else {
        print_tree_layers(t->left, level - 1);
        print_tree_layers(t->right, level - 1);
    }
}

int main(void)
{
    tree t = NULL;
    int h = 0, i;

    add_node(&t, 5);
    add_node(&t, 2);
    add_node(&t, 7);
    add_node(&t, 1);
    add_node(&t, -2);
    add_node(&t, 4);
    add_node(&t, 0);
    add_node(&t, 8);
    add_node(&t, -6);

    h = height(t);
    printf("Height of tree: %d\n", h);

    for (i = 0; i < h; i++) {
        printf("Layer %d: ", i);
        print_tree_layers(t, i);
        putchar('\n');
    }

    destruct_tree(t);

    return 0;
}