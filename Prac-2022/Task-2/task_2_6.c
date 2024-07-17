/*
 * +<неотрицательное целое> -> добавляется в дерево поиска, если нет
 * -<неотрицательное целое> -> удаляется из дп, если есть
 * ?<неотрицательное целое> -> печатается в stdout с новой строки, пробел,
 * yes, если присутствует, no - отсутствует
 * 
 * свойство узла двоичного дерева поиска:
 * у всех узлов левого поддерева значения ключей меньше, чем у рассматриваемого
 * у всех узлов правого поддерева значения ключей больше, чем у рассматриваемого
*/

#include <stdio.h>
#include <stdlib.h>

enum { MAX_LEN = 100 };

typedef struct tNode *tree;    /* указатель на звено дерева*/
typedef struct tNode {         /* дерево состоит из трех полей: */
        unsigned int key;      /* ключ - неотрицательное целое число */
        tree left;             /* левое поддерево */
        tree right;            /* правое поддерево */
} tnode;

/* конвертирует строку в неотрицательное целое число */
unsigned strToUns(char str[]) {
    unsigned int res = 0, i = 0;
    while (str[i] != '\0') {
		res *= 10;
        res += str[i] - '0';
		++i;
	}
    return res;
}

/* рекурсивная печать дерева для отладки */
void printTree(tree t) {
    if (t == NULL)
        return;
    printTree(t->left);
    printf("%u ", t->key);
    printTree(t->right);
} 

/* рекурсивное удаление всего дерева */
void destructTree(tree t) {
    if (t == NULL)
        return;
    destructTree(t->left);
    destructTree(t->right);
    free(t);
}

/* добавить элемент в дерево*/
void addNode(tree *t, unsigned int number) {
    if (!*t) {
        *t = malloc(sizeof(tnode));
        (*t)->key = number;
        (*t)->left = (*t)->right = NULL;
    }
    /* рекурсивный поиск места в двоичном дереве */
    if (number < (*t)->key)
        addNode(&(*t)->left, number);
    if (number > (*t)->key)
        addNode(&(*t)->right, number);
    return;
}

/* поиск элемента в дереве */
int searchNode(tree t, unsigned int number) {
    if (t == NULL)
        return 0;
    if (t->key == number)
        return 1;
    if (number > t->key)
        return(searchNode(t->right, number));
    else
        return(searchNode(t->left, number));
}

/* поиск максимума в дереве поиска */
unsigned int maximumNode(tree t) {
    while (t && t->right != NULL)
        t = t->right;
    return t->key;
}

void destructNode(tree *t, unsigned int number) {
    tree q;
    unsigned int max;
    if (*t == NULL)
        return;
    if ((*t)->key == number) {
        /* лист */
        if ((*t)->left==NULL && (*t)->right==NULL) {
            q = *t;
            *t = NULL;
            free(q);
            return;
        /* есть только правое поддерево */
        } else if ((*t)->left==NULL) {
            q = *t;
            *t = q->right;
            free(q);
            return ;
        /* есть только левое поддерево*/
        } else if ((*t)->right==NULL) {
            q = *t;
            *t = q->left;
            free(q);
            return ;
        /* есть оба поддерева */
        } else {
            (*t)->key = max = maximumNode((*t)->left);
            destructNode(&(*t)->left, max);
            return;
        }
    }
    if (number > (*t)->key)
        destructNode(&(*t)->right, number);
    else
        destructNode(&(*t)->left, number);
}

int main(void) {
    char c, sign;
    int i;
    unsigned int number;
    char buf[MAX_LEN];
    tree searchTree = NULL;
    /* считали знак, пропустили все пробельные последовательности и закончили свою работу, если наступил конец файла */
    while ((c = getchar()) != EOF && (c == '+' || c == '-' || c == '?')) {
        /* считываем число: также пропускаем все ПП */
        sign = c;
        i = 0;
        while ((c = getchar()) != EOF && (c != '\n') && (c != '\t') && (c != ' ') && (i < MAX_LEN - 1))
            buf[i++] = c;
        buf[i] = '\0';
        number = strToUns(buf);
        //printf("sign = %c number = %u\n", sign, number);
        if (sign == '+') {
            addNode(&searchTree, number);
            //printf("added the node\n");
        } else if (sign == '-') {
            destructNode(&searchTree, number);
            //printf("deleted the node\n");
        } else if (sign == '?')
            printf("%u %s\n", number, searchNode(searchTree, number) ? "yes" : "no");          
    //printf("Tree: ");
    //printTree(searchTree);
    //printf("\n");
    }
    destructTree(searchTree);
    return 0;
}