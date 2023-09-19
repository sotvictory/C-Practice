#include <stdio.h>

/* calculates the length of a string */
int mystrlen(const char *s)
{
    int i = 0;

    while (s[i] != '\0') {
        i++;
    }

    return i;
}

/* reverse a string */
void reverse(char *s)
{
    int i = 0, len = mystrlen(s);
    char tmp;

    for (i = 0; i < len / 2; i++) {
        tmp = s[i];
        s[i] = s[len - i - 1];
        s[len - i - 1] = tmp;
    }
}

/* Написать функцию
! 3) cdelete (s, c) - удаляет из заданной строки s все символы c.
4) scandigit( s) – определяет количество символов-цифр в заданной строке
5) scat (s1, s2, s3) – создает из двух, упорядоченных по алфавиту строк s1  и  s2 строку s3, также упорядоченную по алфавиту
! 6) f (s, n, c) – определяет, какой символ (с) чаще других встречается в строке (s) и сколько раз (n) он в нее входит 
! 7) mystrcmp(s1, s2)
*/

int main(void)
{
    char str[] = "These are functions for processing strings"; // length 42 not including zero

    printf("The length of string is %d\n", mystrlen(str));

    reverse(str);
    printf("Reversed string: %s\n", str);
    reverse(str); // return to its original form
    printf("Double reversed string: %s\n", str);

    return 0;
}