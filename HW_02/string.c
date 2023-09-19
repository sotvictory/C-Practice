#include <stdio.h>
#include <ctype.h>

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

/* compare two strings `s1` and `s2`
 * return 0 if the `s1` and `s2` are equal
 * return a negative value if `s1` is less than `s2`
 * return a positive value if `s1` is greater than `s2` */
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

/* determine which character `c` occurs most often in a string `s` and its quantity `n` */
void frequency(const char *s, int *n, char *c)
{
    int cnt[256] = {0};
    int len = mystrlen(s);
    int max_cnt = 0, i;
    unsigned char cur, max_char;

    /* handle empty string */
    if (len == 0) {
        fprintf(stderr, "The string is empty\n");
        return;
    }

    for (i = 0; i < len; i++) {
        cur = s[i]; 
        cnt[cur]++;
        if (cnt[cur] > max_cnt) {
            max_cnt = cnt[cur];
            max_char = s[i];
        }
    }

    *n = max_cnt;
    *c = max_char;
}

/* remove all characters `c` from a string `s` */
void cdelete(char *s, char c)
{
    char *p = s, *q = s;

    while (*p) {
        if (*p!= c) {
            *q = *p;
            q++;
        }
        p++;
    }
    *q = '\0';
}

/* return the number of digits in a string */
int scandigit(const char *s)
{
    int cnt = 0;

    while (*s) {
        if (isdigit(*s)) {
            cnt++;
        }
        s++;
    }

    return cnt;
}

/* create from two ordered strings `s1` and `s2` a string `s3`, also ordered */
void scat(const char *s1, const char *s2, char *s3)
{
    int len_1 = mystrlen(s1), len_2 = mystrlen(s2);
    int i = 0, j = 0, k = 0;

    /* it is possible to iterate over both lines */
    while (i < len_1 && j < len_2) {
        if (s1[i] <= s2[j]) {
            s3[k] = s1[i];
            i++;
        } else {
            s3[k] = s2[j];
            j++;
        }
        k++;
    }

    /* `s2` is over, but `s1` is not */
    while (i < len_1) {
        s3[k] = s1[i];
        i++;
        k++;
    }

    /* `s1` is over, but `s2` is not */
    while (j < len_2) {
        s3[k] = s2[j];
        j++;
        k++;
    }

    /* both lines are over */
    s3[k] = '\0';
}

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