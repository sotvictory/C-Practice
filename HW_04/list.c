# include <stdio.h>
# include <stdlib.h>

typedef struct Node *link;

typedef struct Node { 
        int elem;
        link next;
} node;

typedef link list;

list list_create(int number)
{
    link cur;
    list res;

    res = cur = (link) malloc(sizeof(node));
    cur->elem = number;
    cur->next = NULL;

    return res;
}

void list_print(list p)
{
    while(p != NULL) {
        if (p->next != NULL) {
            printf("%d ", p->elem);
            p = p->next;
        } else {
            printf("%d\n", p->elem);
            break;
        }
    }
}

int main(void)
{
    list lst;

    lst = list_create(5);
    list_print(lst);

    return 0;
}