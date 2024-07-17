#include <stdio.h>
#include <ctype.h>
#include "string.h"

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