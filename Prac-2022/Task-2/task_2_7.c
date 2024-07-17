/* 
 * Вычисление значения выражения, содержащего цифры '0'-'9', знаки
 * операций '+', '*', и скобки '(', ')'. Предполагается, что коды цифр
 * упорядочены по возрастанию цифр и спреведливо равенство '9'-'0' == 9
 * как, например, в ASCII кодировке
*/

#include <stdio.h>
#include <setjmp.h>

jmp_buf begin; /* точка начала диалога с пользователем */
char curlex; /* текущая лексема */

void getlex(void); /* выделяет из входного потока очередную лексему */
int expr(void); /* распознает выражение и вычисляет его значение */
int add(void); /* распознает слагаемое и вычисляет его значение */
int mult(void); /* распознает множитель и вычисляет его значение */
int degexp(void); /* распознает степень и вычисляет её значение */
void error(); /* сообщает об ошибке в выражении и передает управление в начало функции main (точка begin) */

/* 
 * Соответствующие БНФ:
 * <выражение> ::= <слагаемое> {+ <слагаемое>} | <слагаемое> {- <слагаемое>}
 * <слагаемое> ::= <степень> {* <степень>} | <степень> {/ <степень>}
 * <степень> ::= <множитель> {^ <множитель>}
 * <множитель> ::= 0|1|2|3|4|5|6|7|8|9| (<выражение>)
*/

void getlex() {
    while ((curlex = getchar()) == ' ');
}

void error(void) {
    printf("\nerror!\n");
    while (getchar() != '\n');
    longjmp(begin, 1);
}

int expr() {
    int e = add();
    while ((curlex == '+') || (curlex == '-')) {
        if (curlex == '+') {
            getlex();
            e += add();
        } else {
            getlex();
            e -= add();
        }
    }
    return e;
}

int mult() {
    int m;
    switch (curlex) {
        case '0': case '1': case '2': case '3': case '4': case '5':
        case '6': case '7': case '8': case '9': 
            m = curlex - '0';
        break;
        case '(': 
            getlex();
        m = expr();
        if (curlex == ')')
            break; /* иначе ошибка - нет закрывающей скобки */
        default:
            error();
    }
    getlex();
    return m;
}

int deg(int base, int degree) {
    int i = 0, result = 1;
    if ((base == 0) && (degree == 0))
        error();
    while (i < degree) {
        result *= base;
        ++i;
    }
    return result;
}

int degexp() {
    int d = mult();
    while (curlex == '^') {
        getlex();
        d = deg(d, degexp());
    }
    return d;
}

int add() {
    int a = degexp(), b = 0;
    while ((curlex == '*' || (curlex == '/'))) {
        if (curlex == '*') {
            getlex();
            a *= degexp();
        } else {
            getlex();
            b = degexp();
            if (b == 0)
                error();
            else
                a /= b;
        } 
    }
    return a;
}

int main(void) {
    int result;
    setjmp(begin);
    printf("==> ");
    getlex();
    result = expr();
    if (curlex != '\n')
        error();
    printf("%d\n", result);
    return 0;
}