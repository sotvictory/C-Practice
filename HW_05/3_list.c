#include <stdio.h>
#include <stdlib.h>

typedef struct Node *link;
typedef link list;

typedef struct Node {
    int elem;
    link next;
} node;

int search_equal(list lst)
{
    link p, q;

    for (p = lst; p != NULL; p = p->next)
        for (q = p->next; q != NULL; q = q->next)
            if (p->elem == q->elem)
                return 1;

    return 0;
}

void add_node(list *lst, int elem) 
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
        printf("%d ", lst->elem);
        lst = lst->next;
    }
}

int main(void)
{
    list lst = NULL;

    add_node(&lst, 1);
    add_node(&lst, 2);
    add_node(&lst, 4);
    add_node(&lst, 4);

    print_list(lst);
    putchar('\n');
    printf("%d\n", search_equal(lst));
    
    return 0;
}
