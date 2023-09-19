ё#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define SIZE 16     /* limit on the list items number */
#define N 256       /* limit on the read characters number */

typedef enum { Start, Word, SingleSpecialChar, DoubleSpecialChar, SingleOr, SingleAnd, SingleGreater, Newline, Stop } vertex;

char **lst = NULL;
char *buf = NULL;

/* initialization */

void nullList(int *sizelist, int *curlist) {
    lst = NULL;
    *sizelist = 0;
    *curlist = 0;
}

void nullBuf(int *sizebuf, int *curbuf) {
    buf = NULL;
    *sizebuf = 0;
    *curbuf = 0;
}

/* support functions */

void memError(void) {
    fprintf(stderr, "Memory reallocation did not occur. Exit");
    exit(-1);
}

int symSet(int c) {
    return c != '\n' && c != ' ' && c != '\t' && c != EOF && c != ';' && 
           c != '<' && c != '(' && c != ')' && c != '|' && c != '&' && c != '>';
}

/* list creating */

char getSym(int *pos, int *n) 
{

    char c;
    static char str[N];

    if (*n == 0) {
        *pos = 0;
        *n = read(0, str, N);
    }
    if (--*n >= 0) {
        c = str[(*pos)++];
        //fprintf(stderr, "n = %d\n", *n);
        //fprintf(stderr, "Got the symbol %c\n", c);
    }
    else {
        c = EOF;
        //fprintf(stderr, "Got the EOF\n");
    }
    
    return c;
}

void addSym(int *sizebuf, int *curbuf, char c) {
    /* increase the buffer size if necessary */
    if (*curbuf > *sizebuf - 1) {
        buf = realloc(buf, *sizebuf += SIZE);
        if (buf == NULL)
            memError();
    }
    //fprintf(stderr, "Character %c will be placed in position %d buf of %d elements\n", c, *curbuf, *sizebuf);
    buf[(*curbuf)++] = c;
    //fprintf(stderr, "Buffer: %s\n", buf);
}

void addWord(int *sizelist, int *curlist, int *sizebuf, int *curbuf) {
    /* increase the buffer size for writing ’\0’ */
    if (*curbuf > *sizebuf - 1) {
        buf = realloc(buf, *sizebuf += 1);
        if (buf == NULL)
            memError();
    }
    buf[(*curbuf)++] = '\0';
    /* restrict the memory to the word size */
    buf = realloc(buf, *sizebuf = *curbuf);
    if (buf == NULL)
        memError();
    /* increase the array size if necessary */
    if (*curlist > *sizelist - 1) {
        lst = realloc(lst, (*sizelist += SIZE) * sizeof(*lst));
        if (lst == NULL)
            memError();
    }
    lst[(*curlist)++] = buf;
}

void termList(int *sizelist, int *curlist) {
    if (lst == NULL) 
        return;
    if (*curlist > *sizelist - 1) {
        lst = realloc(lst, (*sizelist + 1) * sizeof(*lst));
        if (lst == NULL)
            memError();
    }
    lst[*curlist] = NULL;
    /* restrict the list memory exactly to the list size */
    lst = realloc(lst, (*sizelist = *curlist + 1) * sizeof(*lst));
    if (lst == NULL)
        memError();
    //printf("List of %d elements has been completed\n", sizelist-1);
}

/* final processing: sorting, printing, and freeing memory */

/* if s < t, it returns -1
 * if s > t, it returns +1
 * if s = t, it returns 0 */
int myStrcmp(char *s, char *t) {
    int i;
    for (i = 0; s[i] == t[i]; i++) {
        if (s[i] == '\0') 
            return 0;
    }
    if (s[i] < t[i])
        return -1;
    else
        return 1;
}

void swap(char **a, char **b) 
{
    char *c = *a;
    *a = *b;
    *b = c;
}

void sort(int sizelist) {
    int i, j;
    for (i = 1; i < sizelist - 1; i++)
        for (j = i; j > 0 && (myStrcmp(lst[j], lst[j - 1]) == -1); j--) 
            swap(&lst[j], &lst[j - 1]);
}

void printList(int sizelist) {
    int i;
    if (lst == NULL) 
        return;
    printf("%d\n", sizelist - 1);
    for (i = 0; i < sizelist - 1; i++)
        printf("%s\n", lst[i]);
}

void clearList(int *sizelist, int *curlist) {
    int i;
    *sizelist = 0;
    *curlist = 0;
    if (lst == NULL) 
        return;
    for (i = 0; lst[i] != NULL; ++i)
        free(lst[i]);
    free(lst);
    lst = NULL;
}

int graph(void) {
    static int sizebuf = 0, sizelist = 0, curbuf = 0, curlist = 0, n = 0, pos, c;
    vertex V = Start;

    c = getSym(&pos, &n);
    nullList(&sizelist, &curlist);
    while (1)
        switch(V) {

            case Start:
                if (c == ' ' || c == '\t')
                    c = getSym(&pos, &n);
                else if (c == EOF) {
                    termList(&sizelist, &curlist);
                    printList(sizelist);
                    clearList(&sizelist, &curlist);
                    V = Stop;
                } else if (c == '\n') {
                    termList(&sizelist, &curlist);
                    //fprintf(stderr, "Source list: \n");
                    printList(sizelist);
                    sort(sizelist);
                    //fprintf(stderr, "Sorted list: \n");
                    printList(sizelist);
                    //fprintf(stderr, "\n");
                    V = Newline;
                    c = getSym(&pos, &n);
                } else {
                    nullBuf(&sizebuf, &curbuf);
                    addSym(&sizebuf, &curbuf, c);
                    if (c == ';' || c == '<' || c == '(' || c == ')')
                        V = SingleSpecialChar;
                    else if (c == '|')
                        V = SingleOr;
                    else if (c == '&')
                        V = SingleAnd;
                    else if (c == '>')
                        V = SingleGreater;
                    else
                        V = Word;
                    c = getSym(&pos, &n);
                }
                break;

            case Word:
                if (symSet(c)) {
                    addSym(&sizebuf, &curbuf, c);
                    c = getSym(&pos, &n);
                } else {
                    V = Start;
                    addWord(&sizelist, &curlist, &sizebuf, &curbuf);
                }
                break;

            case SingleOr:
                if (c == '|') {
                    addSym(&sizebuf, &curbuf, c);
                    c = getSym(&pos, &n);
                    V = DoubleSpecialChar;
                } else {
                    V = Start;
                    addWord(&sizelist, &curlist, &sizebuf, &curbuf);
                }
                break;

            case SingleAnd:
                if (c == '&') {
                    addSym(&sizebuf, &curbuf, c);
                    c = getSym(&pos, &n);
                    V = DoubleSpecialChar;
                } else {
                    V = Start;
                    addWord(&sizelist, &curlist, &sizebuf, &curbuf);
                }
                break;

            case SingleGreater:
                if (c == '>') {
                    addSym(&sizebuf, &curbuf, c);
                    c = getSym(&pos, &n);
                    V = DoubleSpecialChar;
                } else {
                    V = Start;
                    addWord(&sizelist, &curlist, &sizebuf, &curbuf);
                }
                break;

            case SingleSpecialChar:
            case DoubleSpecialChar:
                V = Start;
                addWord(&sizelist, &curlist, &sizebuf, &curbuf);
                break;

            case Newline:
                clearList(&sizelist, &curlist);
                V = Start;
                break;

            case Stop:
                exit(0);
                break;
        }
}