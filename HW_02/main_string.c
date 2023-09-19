#include <stdio.h>
#include "string.h"

int main(void)
{
    char str[] = "These are functions for processing strings"; // length 42 not including zero
    char str1[] = "These are functions";
    char str2[] = "These are functions for processing some strings";
    char str3[] = "Digits 1, 2, 3. di5gi64t"; // 6 digits
    char str4[] = "ac";
    char str5[] = "bfxz"; //
    char empty_str[] = "";
    int n = 0;
    char c;

    printf("The length of string is %d\n\n", mystrlen(str)); // 42

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
    printf("The result of mystrcmp for strings 1 and 3: %d\n\n", mystrcmp(str, str2)); // 1 < 3

    frequency(empty_str, &n, &c);
    if (n != 0)
        printf("Most frequent character is '%c' and it occurs %d times.\n\n", c, n); // empty
    frequency(str, &n, &c);
    if (n != 0)
        printf("Most frequent character is '%c' and it occurs %d times.\n\n", c, n); // 's', 6 times

    printf("String before: %s\n", str);
    cdelete(str, 's');
    printf("String after: %s\n\n", str);

    printf("String: %s\n", str3);
    printf("There are %d digits in the string\n", scandigit(str3)); // 6
    printf("String: %s\n", str);
    printf("There are %d digits in the string\n\n", scandigit(str)); // 0

    printf("First string: %s\n", str4);
    printf("Second string: %s\n", str5);
    scat(str4, str5, empty_str);
    printf("Result of concatenation: %s\n", empty_str);

    return 0;
}