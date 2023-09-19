#include <stdio.h>

/* calculate the length of a string */
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

/* compare two strings 
 * return 0 if the s1 and s2 are equal
 * return a negative value if s1 is less than s2
 * return a positive value if s1 is greater than s2 */
int mystrcmp(const char *s1, const char *s2)
{
    int i = 0;

    while (s1[i] && s2[i]) {
        if (s1[i] - s2[i])
            return (s1[i] - s2[i]);
        i++;
    }
    return (s1[i] - s2[i]);
}

/* Написать функцию
! 3) cdelete (s, c) - удаляет из заданной строки s все символы c.
4) scandigit( s) – определяет количество символов-цифр в заданной строке
5) scat (s1, s2, s3) – создает из двух, упорядоченных по алфавиту строк s1 и s2 строку s3, также упорядоченную по алфавиту
! 6) f (s, n, c) – определяет, какой символ (с) чаще других встречается в строке (s) и сколько раз (n) он в нее входит 
*/

int main(void)
{
    char str[] = "These are functions for processing strings"; // length 42 not including zero
    char str1[] = "These are functions";
    char str2[] = "These are functions for processing some strings";

    printf("The length of string is %d\n\n", mystrlen(str));

    reverse(str);
    printf("Reversed string: %s\n", str);
    reverse(str); // return to its original form
    printf("Double reversed string: %s\n\n", str);

    printf("First string: %s\n", str);
    printf("Second string: %s\n", str1);
    printf("Third string: %s\n", str2);
    printf("The result of mystrcmp for strings 1 and 1: %d\n", mystrcmp(str, str)); // equal
    printf("The result of mystrcmp for strings 1 and 2: %d\n", mystrcmp(str, str1)); // 1 > 2
    printf("The result of mystrcmp for strings 2 and 1: %d\n", mystrcmp(str1, str)); // 1 > 2
    printf("The result of mystrcmp for strings 1 and 3: %d\n", mystrcmp(str, str2)); // 1 < 3

    return 0;
}