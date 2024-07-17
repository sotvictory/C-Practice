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

list list_destruct(list lst)
{
    link cur;

    while (lst != NULL) {
        cur = lst;
        lst = lst->next;
        free(cur);
    }

    return NULL;
}

list list_node_delete(int number, list lst)
{
    link prev = NULL, cur = lst;

    while (cur != NULL && cur->elem != number) {
        prev = cur;
        cur= cur->next;
    }

    if (cur == NULL) {
        return lst;
    }

    if (prev == NULL) {
        lst = cur->next;
    } else {
        prev->next = cur->next;
    }

    free(cur);
    return lst;
}

list list_node_delete_all(int number, list lst)
{
    link prev = NULL, cur = lst, tmp;

    while (cur != NULL) {
        if (cur->elem == number) {
            if (prev == NULL) {
                lst = cur->next;
            } else {
                prev->next = cur->next;
            }
            tmp = cur;
            cur = cur->next;
            free(tmp);
        } else {
            prev = cur;
            cur = cur->next;
        }
    }

    return lst;
}

int main(void)
{
    list lst = NULL;
    list_print(lst);
    lst = list_create(3);
    list_print(lst);

    list_insert_end(3, lst);
    list_insert_end(2, lst);
    list_insert_end(6, lst);
    list_insert_end(3, lst);
    list_insert_end(2, lst);
    list_insert_end(3, lst);
    list_print(lst);
    
    lst = list_node_delete(2, lst);
    lst = list_node_delete_all(3, lst);
    list_print(lst);

    lst = list_destruct(lst);
    list_print(lst);

    return 0;
}