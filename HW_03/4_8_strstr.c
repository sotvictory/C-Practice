#include <stdio.h>
#include <string.h>

char *ststr(const char *s1, const char *s2)
{
    int n = strlen(s2);
    int i = 0;

    while (*s1 != '\0') {
        if (strncmp(s1, s2, n) == 0) {
            return (char *) s1;
        }
        i++;
        s1++;
    }

    return NULL;
}

int main(void)
{
    char s1[] = "This is simple string, but with the word end.";
    char s2[] = "This is simple string";
    char s3[] = "";
    char s4[] = "end";

    char *example_1 = strstr(s1, s4);
    char *example_2 = strstr(s2, s4);
    char *example_3 = strstr(s3, s4);

    if (example_1)
        printf("%s\n", example_1);
    else
        printf("Not found (NULL)\n");

    if (example_2)
        printf("%s\n", example_2);
    else
        printf("Not found (NULL)\n");

    if (example_3)
        printf("%s\n", example_3);
    else
        printf("Not found (NULL)\n");

    return 0;
}
