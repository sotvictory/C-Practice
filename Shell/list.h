#ifndef LIST_H
#define LIST_H

typedef char **list;

list dolist(int *sizelistPoint);
list setvar(list lst, int sizelist);
void printlist(list lst, int sizelist);
list clearlist(list lst, int *sizelistPoint);

#endif /* LIST_H */