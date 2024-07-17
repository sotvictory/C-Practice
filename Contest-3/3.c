#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void back_max(char *str)
{
    int total_len = 0, max_len = 0, max_token_len = 0;
    int num, max_num;

    char *token = strtok(str, ",");
    if (token == NULL) {
        return;
    } else {
        max_num = atoi(token);
        total_len += strlen(token) + 1;
    }

    char *buf = malloc(strlen(str) + 1);
    if (buf == NULL) {
        exit(1);
    }
    strcat(buf, token);
    strcat(buf, ",");

    token = strtok(NULL, ",");
    while (token != NULL) {
        strcat(buf, token);
        strcat(buf, ",");
        num = atoi(token);
        if (num >= max_num) {
            max_num = num;
            max_len = total_len;
            max_token_len = strlen(token) + 1;
        }
        total_len += strlen(token) + 1;
        token = strtok(NULL, ",");
    }

    char *new_str = (char *)malloc(strlen(str) + 1);
    if (new_str == NULL) {
        exit(1);
    }
    memcpy(new_str, buf, max_len);
    memcpy(new_str + max_len, buf + max_len + max_token_len, total_len - max_len - max_token_len);
    memcpy(new_str + total_len - max_token_len, buf + max_len, max_token_len);
    memcpy(str, new_str, strlen(buf) + 1);

    free(buf);
    free(new_str);

    return;
}

int main(void)
{
    char str[] = "5,4,5,3,2,1,";
    back_max(str);
    printf("%s\n", str);

    return 0;
}