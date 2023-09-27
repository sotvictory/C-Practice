#ifndef STRING_H
#define STRING_H

int mystrlen(const char *s);
void reverse(char *s);
int mystrcmp(const char *s1, const char *s2);
void frequency(const char *s, int *n, char *c);
void cdelete(char *s, char c);
int scandigit(const char *s);
void scat(const char *s1, const char *s2, char *s3);

#endif /* STRING_H */