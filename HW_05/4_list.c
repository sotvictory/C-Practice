#include <stdio.h>
#include <stdlib.h>

typedef struct Node *link;
typedef link list;

typedef struct Node {
    double elem;
    link next;
} node;

void add_node(list *lst, double elem) 
{
    link new_node = malloc(sizeof(node));
    new_node->elem = elem;
    new_node->next = NULL;
    
    if (*lst == NULL) {
        *lst = new_node;
    } else {
        link p = *lst;
        while (p->next != NULL) {
            p = p->next;
        }
        p->next = new_node;
    }
}

void print_list(list lst)
{
    while (lst != NULL) {
        printf("%.2f ", lst->elem);
        lst = lst->next;
    }
}

void print_list_reverse(list lst)
{
    if (lst == NULL)
        return;
    print_list_reverse(lst->next);
    printf("%.2f ", lst->elem);
}

void destruct_list(link lst)
{
    link p, q = lst;
    
    while (q != NULL) {
        p = q->next;
        free(q);
        q = p;
    }
}

int main(void)
{
    list lst = NULL;

    add_node(&lst, 1.12);
    add_node(&lst, -2.23);
    add_node(&lst, 3.21);
    add_node(&lst, -4.04);

    print_list(lst);
    putchar('\n');
    print_list_reverse(lst);
    putchar('\n');
    
    destruct_list(lst);

    return 0;
}