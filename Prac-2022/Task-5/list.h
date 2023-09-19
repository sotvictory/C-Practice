#ifndef LIST_H
#define LIST_H

typedef ... List

List list;



extern jmp_buf begin;

void print_list(list *);
void free_list(list *);
void build_list(list *, char *);
void change_lst(list *);

#endif
