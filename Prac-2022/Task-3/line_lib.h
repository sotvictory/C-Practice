#ifndef LINE_LIB_H
#define LINE_LIB_H

void nullList(int *sizelist, int *curlist);
void nullBuf(int *sizebuf, int *curbuf);
void memError(void);
int symSet(int c);
char getSym(int *pos, int *n);
void addSym(int *sizebuf, int *curbuf, char c);
void addWord(int *sizelist, int *curlist, int *sizebuf, int *curbuf);
void termList(int *sizelist, int *curlist);
int myStrcmp(char *s, char *t);
void swap(char **a, char **b);
void sort(int sizelist);
void printList(int sizelist);
void clearList(int *sizelist, int *curlist);
int graph();

#endif /* LINE_LIB_H */