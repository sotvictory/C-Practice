# include <stdio.h>
# include <stdlib.h>

typedef struct Node *link;
typedef link list;

typedef struct Node { 
        int elem;
        link next;
} node;

list list_create(int number)
{
    list lst = (link) malloc(sizeof(node));
    lst->elem = number;
    lst->next = NULL;

    return lst;
}

void list_print(list lst)
{
    if (lst == NULL) {
        fprintf(stderr, "The list is empty\n");
        return;
    }

    while(lst != NULL) {
        if (lst->next != NULL) {
            printf("%d ", lst->elem);
            lst = lst->next;
        } else {
            printf("%d\n", lst->elem);
            break;
        }
    }
}

void list_insert_end(int number, list lst)
{
    link cur = (link) malloc(sizeof(node));
    cur->elem = number;
    cur->next = NULL;

    while (lst->next != NULL) {
        lst = lst->next;
    }

    lst->next = cur;
}

void list_destruct(list lst)
{
    link cur;

    while (lst != NULL) {
        cur = lst;
        lst = lst->next;
        free(cur);
    }
}

int main(void)
{
    list lst = list_create(5);

    list_insert_end(3, lst);
    list_insert_end(6, lst);
    list_insert_end(0, lst);
    list_insert_end(1, lst);

    list_print(lst);
    list_destruct(lst);

    return 0;
}