#include <stdio.h>
#include <stdlib.h>

#define n 1000

/* список букв */
typedef struct Word *wordLink; /* указатель на слово */
typedef struct Word {          /* слово состоит из двух полей: */
        char letter;           /* тип элемента списка - буква */
        wordLink next;         /* указатель на следующую букву */
} word;
typedef wordLink wordList;     /* список задается указателем на слово */

/* список слов */
typedef struct Node *link;     /* указатель на звено */
typedef struct Node {          /* звено состоит из двух полей: */
        wordLink elem;         /* тип элемента списка - указатель на слово */
        link next;             /* указатель на следующее звено */
} node;
typedef link list;             /* список задается указателем на звено */

/* печать слова */
void printWord(wordList p) {
    while (p) {
        putchar(p->letter);
        p = p->next;
    }
}

/* печать списка слов */
void print(list p) {
    /* первое слово - особый случай, тк перед ним не нужен пробел */
    if (p) {
        printWord(p->elem);
        p = p->next;
    }
    while (p) {
        printf(" ");
        printWord(p->elem);
        p = p->next;
    }
    printf("\n");
}

/* создание слова */
void createWord(char *s, wordList *p) {
    wordList cur, res;
    /* пустой список */
    if (!*s) {
        *p = NULL;
        return;
    }
    /* если не пустой */
    res = cur = malloc(sizeof(word));
    cur->letter = *s++;
    while (*s) {
        cur = cur->next = malloc(sizeof(word));
        cur->letter = *s++;
    }
    cur->next = NULL;
    *p = res;
    return;
}

/* создание списка слов */
list create() {
    list cur, res;
    char str[n];
    /* есть 1 слово */
    if (scanf("%s", str) == 1) {
        res = cur = malloc(sizeof(node));
        createWord(str, &cur->elem);
    } else
    /* нет слов */
        return NULL;
    /* > 1 слова */
    while (scanf("%s", str) == 1) {
        cur = cur->next = malloc(sizeof(node));
        createWord(str, &cur->elem);
    }
    cur->next = NULL;
    return res;
}

/* проверка слов на равенство слов*/
int isEqual(wordList a, wordList b) {
    /* список пустой */
    if (a == NULL && b == NULL)
        return 1;
    /* список непустой */
    while (a->letter == b->letter) {
        if (a->next == NULL && b->next == NULL)
        /* все элементы равны и списки закончились */
            return 1;
        /* один из списков закончился раньше */
        else if (a-> next == NULL || b->next == NULL)
            return 0;
        a = a->next;
        b = b->next;
    }
    /* встретили неравные элементы */
    return 0; 
}

/* удаление слова */
void destructWord(wordList p) {
    wordLink q;
    while (p) { 
        q = p;
        p = p->next;
        free(q);
    }
    return;
}

/* удаление списка слов */
void destruct(list p) {
    link q;
    while (p) {
        q = p;
        p = p->next;
        destructWord(q->elem);
        free(q);
    }
    return;
}

/* нахождение последнего слова */
wordList lastWord(list p) {
    /* список пустой */
    if (!p)
        return NULL;
    else {
    /* список непустой */
        while (p->next) 
            p = p->next;
        return p->elem;
    }
}

/* удаление слов, совпадающих с последнем, само последнее остается */
void destructLastWord(list *p, wordList w) {
    list q;
    if (*p) {
        /* если нет слова */
        if ((*p)->next == NULL)
            return;
        else
            /* сравниваем, пока список не кончается*/
            while ((*p)->next)
                if (isEqual((*p)->elem, w)) {
                    destructWord((*p)->elem);
                    q = (*p);
                    *p = q->next;
                    free(q);
                } else
                    p = &(*p)->next;
    }
    return;
}

int main(void) {
    list listWord;
    listWord = create();
    destructLastWord(&listWord, lastWord(listWord));
    print(listWord);
    destruct(listWord);
    return 0;
}